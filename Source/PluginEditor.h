/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#include "SpeedSlider.h"

//==============================================================================
/**
*/
class PluginEditor  : public AudioProcessorEditor, public SliderListener
{
public:
    PluginEditor (PluginProcessor&);
    ~PluginEditor();

    //==============================================================================
    //void createEditor() override;
    void paint (Graphics&) override;
    void resized() override;

private:
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    
    std::shared_ptr<juce::AudioProcessorEditor> editor;
    
    //ScopedPointer<Slider> speed;
    std::shared_ptr<SpeedSlider> speed;
    //ScopedPointer<Label> label;
    //std::unique_ptr<Label> label;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SpinAudioProcessor& processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpinAudioProcessorEditor)
};
