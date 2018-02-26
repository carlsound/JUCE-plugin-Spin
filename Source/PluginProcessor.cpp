/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
SpinAudioProcessor::SpinAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
	oscillator_1_ = std::make_shared<maxiOsc>();
	oscillator_2_ = std::make_shared<maxiOsc>();
	oscillator_settings_ = std::make_shared<maxiSettings>();
	//
	oscillator_1_->phaseReset(0.0);
	oscillator_2_->phaseReset(double_Pi / 2);
}

SpinAudioProcessor::~SpinAudioProcessor()
{
	oscillator_1_ = nullptr;
	oscillator_2_ = nullptr;
	oscillator_settings_ = nullptr;
}

//==============================================================================
const String SpinAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SpinAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SpinAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SpinAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SpinAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SpinAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SpinAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SpinAudioProcessor::setCurrentProgram (int index)
{
}

const String SpinAudioProcessor::getProgramName (int index)
{
    return {};
}

void SpinAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SpinAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    //
    //audioEngine.prepare ({ sampleRate, (uint32) samplesPerBlock, 2 });
	oscillator_settings_->sampleRate = sampleRate;
	oscillator_settings_->bufferSize = samplesPerBlock;
}

void SpinAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SpinAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void SpinAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    //
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    /*
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);
        // ..do something to the data...
    }
     */
    //audioEngine.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());
	//
	// left channel
	float *channel_1_read_ = buffer.getReadPointer(channel_1_);
	float *channel_1_write_ = buffer.getReadPointer(channel_1_);
	//
	//right channel
	if (totalNumInputChannels > 1)
	{
		float *channel_2_read_ = buffer.getReadPointer(channel_2_);
		channel_2_input_ = true;
	}
	if (totalNumOutputChannels > 1)
	{
		float *channel_2_write_ = buffer.getReadPointer(channel_2_);
		channel_2_output = true;
	}
	//
	for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
	{
		float waveform_1_ = oscillator_1_->sinewave(frequency_);
		float waveform_2_ = oscillator_2_->sinewave(frequency_);
		channel_1_write_[sample] = channel_1_read_[sample] * waveform_1_;
		if (channel_2_input_ && channel_2_output)
		{

		}
		else
		{

		}
	}
}

//==============================================================================
bool SpinAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SpinAudioProcessor::createEditor()
{
    return new SpinAudioProcessorEditor (*this);
}

//==============================================================================
void SpinAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SpinAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpinAudioProcessor();
}
