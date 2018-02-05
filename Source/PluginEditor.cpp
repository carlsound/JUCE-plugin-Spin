/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
PluginEditor::PluginEditor (AudioProcessor& p): AudioProcessorEditor (&p), processor (p)
{
    //editor(new juce::AudioProcessorEditor(&p));
    
    //std::unique_ptr<SpeedSlider> speed(new SpeedSlider);
    addAndMakeVisible(speed(new SpeedSlider));
    speed->addListener(this);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    //setSize (400, 300);
}

PluginEditor::~PluginEditor()
{
    speed = nullptr;
    //label = nullptr;
}

//==============================================================================
/*
void PluginEditor::createEditor() override
{
    
}
 */

void PluginEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    //g.setColour (Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void PluginEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void PluginEditor::sliderValueChanged (Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == speed)
    {
        //processor.panSpeed->setValue((float)slider->getValue());
        //processor.setPanSpeed((float)slider->getValue());
    }
}
