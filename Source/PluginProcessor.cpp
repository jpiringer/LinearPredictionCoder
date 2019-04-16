/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define MAX_DELAY_LENGTH 10.0f

//==============================================================================
LinearPredictionCoderAudioProcessor::LinearPredictionCoderAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#else
    :
#endif
        parameters (*this, nullptr, Identifier ("LinearPredictionCoder"),
              {
                  std::make_unique<AudioParameterFloat> ("semitones",            // parameterID
                                                         "Pitch Shift Semitones",            // parameter name
                                                         NormalisableRange<float>(-24.f,              // minimum value
                                                         24.f, 1.0f),              // maximum value
                                                         0.f),             // default value
                  std::make_unique<AudioParameterFloat> ("cents",            // parameterID
                                                         "Pitch Shift Cents",            // parameter name
                                                         NormalisableRange<float>(-100.0f,              // minimum value
                                                                                  100.f, 1.f),              // maximum value
                                                         0.f),             // default value
                  std::make_unique<AudioParameterFloat> ("delayAmount",
                                                         "Delay Amount",
                                                         NormalisableRange<float>(0.f, 1.f, 0.001f),
                                                         0.0f),
                  std::make_unique<AudioParameterFloat> ("delayTime",
                                                         "Delay Time",
                                                         NormalisableRange<float>(0.001f, MAX_DELAY_LENGTH, 0.001f),
                                                         0.1f)
              })
{
    //addParameter (semitones = new AudioParameterFloat ("semitones", "Pitch Shift Semitones", -24, 24, 1));
    semitones = parameters.getRawParameterValue("semitones");
    cents = parameters.getRawParameterValue("cents");
    delayTime = parameters.getRawParameterValue("delayTime");
    delayAmount = parameters.getRawParameterValue("delayAmount");
    
    sp_create(&sp);
    for (size_t i = 0; i < MAX_CHANNELS; i++) {
        sp_pshift_create(&pshift[i]);
        sp_pshift_init(sp, pshift[i]);
        sp_lpc_create(&LPCSP[i]);
        sp_lpc_init(sp, LPCSP[i], 512);
        sp_dynamic_delay_create(&delay[i]);
        sp_dynamic_delay_init(sp, delay[i], MAX_DELAY_LENGTH);
        delay[i]->feedback = 0;
        zBuffer[i] = 0;
    }
}

LinearPredictionCoderAudioProcessor::~LinearPredictionCoderAudioProcessor() {
    for (size_t i = 0; i < MAX_CHANNELS; i++) {
        sp_pshift_destroy(&pshift[i]);
        sp_lpc_destroy(&LPCSP[i]);
        sp_dynamic_delay_destroy(&delay[i]);
    }
    sp_destroy(&sp);
}

//==============================================================================
const String LinearPredictionCoderAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LinearPredictionCoderAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LinearPredictionCoderAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LinearPredictionCoderAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LinearPredictionCoderAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LinearPredictionCoderAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LinearPredictionCoderAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LinearPredictionCoderAudioProcessor::setCurrentProgram (int index)
{
}

const String LinearPredictionCoderAudioProcessor::getProgramName (int index)
{
    return {};
}

void LinearPredictionCoderAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void LinearPredictionCoderAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void LinearPredictionCoderAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NewProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const {
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono())
     //&& layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void LinearPredictionCoderAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages) {
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    //auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    /*for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
        buffer.clear (i, 0, buffer.getNumSamples());
    }*/
    
    bool doShift = *semitones != 0 || *cents != 0;

    if (doShift) {
        for (size_t i = 0; i < MAX_CHANNELS; i++) {
            *pshift[i]->shift = *semitones+*cents/100.0f;
        }
    }
    for (size_t i = 0; i < MAX_CHANNELS; i++) {
        sp_dynamic_delay_set_time(sp, delay[i], *delayTime);
    }

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        float *input = (float *)buffer.getReadPointer(channel);
        float *output = buffer.getWritePointer(channel);
        
        for (int i = 0; i < buffer.getNumSamples(); i++) {
            float delayOutput;
            if (*delayAmount > 0) {
                sp_dynamic_delay_compute(sp, delay[channel], &zBuffer[channel], &delayOutput);
                input[i] += delayOutput * *delayAmount;
            }
            if (doShift) {
                float shifted;
                sp_pshift_compute(sp, pshift[channel], &input[i], &shifted);
                sp_lpc_compute(sp, LPCSP[channel], &shifted, &output[i]);
            }
            else {
                sp_lpc_compute(sp, LPCSP[channel], &input[i], &output[i]);
            }
            zBuffer[channel] = output[i];
        }
    }
}
void LinearPredictionCoderAudioProcessor::setPitchShift(float _semitones) {
    *semitones = _semitones;
}

//==============================================================================
bool LinearPredictionCoderAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* LinearPredictionCoderAudioProcessor::createEditor() {
    //return new LinearPredictionCoderAudioProcessorEditor (*this, parameters);
    return new GenericAudioProcessorEditor (this);
}

//==============================================================================
void LinearPredictionCoderAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void LinearPredictionCoderAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new LinearPredictionCoderAudioProcessor();
}
