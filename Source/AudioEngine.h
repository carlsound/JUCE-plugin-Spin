/*
  ==============================================================================

    AudioEngine.h
    Created: 26 Nov 2017 10:14:00am
    Author:  John Carlson

  ==============================================================================
*/
//
#pragma once
//
#include "Voice.h"
//
//==============================================================================
class AudioEngine  : public juce::MPESynthesiser
{
public:
    static constexpr size_t maxNumVoices = 4;
    //
    //==============================================================================
    AudioEngine()
    {
        for (size_t i = 0; i < maxNumVoices; ++i) addVoice (new Voice);
        setVoiceStealingEnabled (true);
    }
    //
    //==============================================================================
    void prepare (const juce::dsp::ProcessSpec& spec) noexcept
    {
        setCurrentPlaybackSampleRate (spec.sampleRate);
        for (auto* v : voices) dynamic_cast<Voice*> (v)->prepare (spec);
        fxChain.prepare (spec);
    }
    //
private:
    //==============================================================================
    void renderNextSubBlock (AudioBuffer<float>& outputAudio, int startSample, int numSamples) override
    {
        MPESynthesiser::renderNextSubBlock (outputAudio, startSample, numSamples);
        auto block = juce::dsp::AudioBlock<float> (outputAudio);                         // wrap outputAudio in a juce::dsp::AudioBlock
        auto blockToUse = block.getSubBlock ((size_t) startSample, (size_t) numSamples); // create another juce::dsp::AudioBlock which refers to the section of the previous block defined by startSample and numSamples
        auto contextToUse = juce::dsp::ProcessContextReplacing<float> (blockToUse);      // wrap the above AudioBlock in a juce::dsp::ProcessContextReplacing
        fxChain.process (contextToUse);                                                  // process the fxChain with the above context
    }
    //
    enum{reverbIndex};
    juce::dsp::ProcessorChain<juce::dsp::Reverb> fxChain;
};