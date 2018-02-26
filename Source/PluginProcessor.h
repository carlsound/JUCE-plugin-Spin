/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//#include "AudioEngine.h"
#include "../3rdPartyLibraries/maximilian.h"

//==============================================================================
/**
*/
class SpinAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    SpinAudioProcessor();
    ~SpinAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect () const override;
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
    
    //==============================================================================
    

private:
    //==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpinAudioProcessor)

	//AudioEngine audioEngine;
	std::shared_ptr<maxiOsc> oscillator_1_;
	std::shared_ptr<maxiOsc> oscillator_2_;
	std::shared_ptr<maxiSettings> oscillator_settings_;
	double frequency = 0.0;
	//
	const int channel_1_ = 0;
	const int channel_2_ = 1;
	//
	bool channel_2_input_ = false;
	bool channel_2_output = false;
};
