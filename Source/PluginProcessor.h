/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

extern "C" {
#include <soundpipe.h>
}

#include "dynamic_delay.hpp"

#define MAX_CHANNELS 2

//==============================================================================
/**
*/
class LinearPredictionCoderAudioProcessor  : public AudioProcessor
{
    sp_data *sp;
    sp_pshift *pshift[MAX_CHANNELS];
    sp_lpc *LPCSP[MAX_CHANNELS];
    sp_dynamic_delay *delay[MAX_CHANNELS];
    float zBuffer[MAX_CHANNELS];
    
    AudioProcessorValueTreeState parameters;
    float *semitones = nullptr;
    float *cents = nullptr;
    float *delayTime = nullptr;
    float *delayAmount = nullptr;
    //AudioParameterFloat *semitones;
    
public:
    //==============================================================================
    LinearPredictionCoderAudioProcessor();
    ~LinearPredictionCoderAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    void setPitchShift(float _semitones);

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LinearPredictionCoderAudioProcessor)
};
