/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class LinearPredictionCoderAudioProcessorEditor  :
    public AudioProcessorEditor,
    private Slider::Listener
{
public:
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    
    LinearPredictionCoderAudioProcessorEditor (LinearPredictionCoderAudioProcessor&, AudioProcessorValueTreeState& vts);
    ~LinearPredictionCoderAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    AudioProcessorValueTreeState& valueTreeState;
    
    void sliderValueChanged(Slider *slider) override;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    LinearPredictionCoderAudioProcessor& processor;
    
    Slider pitchShiftAmount;
    std::unique_ptr<SliderAttachment> pitchShiftAmountAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LinearPredictionCoderAudioProcessorEditor)
};
