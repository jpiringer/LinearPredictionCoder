/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LinearPredictionCoderAudioProcessorEditor::LinearPredictionCoderAudioProcessorEditor (LinearPredictionCoderAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), valueTreeState (vts), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (200, 300);
    
    // these define the parameters of our slider object
    /*pitchShiftAmount.setSliderStyle (Slider::LinearBarVertical);
    pitchShiftAmount.setRange(-24, 24, 1.0);
    pitchShiftAmount.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
    pitchShiftAmount.setPopupDisplayEnabled (true, false, this);
    pitchShiftAmount.setTextValueSuffix (" Semitones");
    pitchShiftAmount.setValue(0);
    pitchShiftAmount.addListener(this);
    */
    
    // this function adds the slider to the editor
    pitchShiftAmount.setSliderStyle (Slider::LinearBarVertical);
    pitchShiftAmount.setRange(-24, 24, 1.0);
    pitchShiftAmount.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
    pitchShiftAmount.setPopupDisplayEnabled (true, false, this);
    pitchShiftAmount.setTextValueSuffix (" Semitones");
    addAndMakeVisible (&pitchShiftAmount);
    
    pitchShiftAmountAttachment.reset (new SliderAttachment (valueTreeState, "gain", pitchShiftAmount));
}

LinearPredictionCoderAudioProcessorEditor::~LinearPredictionCoderAudioProcessorEditor()
{
}

//==============================================================================
void LinearPredictionCoderAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Pitch Shift", 0, 0, getWidth(), 30, Justification::centred, 1);
    
}

void LinearPredictionCoderAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    pitchShiftAmount.setBounds (40, 30, 20, getHeight() - 60);
}

void LinearPredictionCoderAudioProcessorEditor::sliderValueChanged(Slider * slider)
{
    //processor.setPitchShift(pitchShiftAmount.getValue());
}
