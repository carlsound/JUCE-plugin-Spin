/*
  ==============================================================================

    Voice.h
    Created: 26 Nov 2017 10:15:03am
    Author:  John Carlson

  ==============================================================================
*/
//
#pragma once
//
#include "Oscillator.h"
//
//==============================================================================
class Voice  : public juce::MPESynthesiserVoice
{
public:
    Voice()
    {
        auto& masterGain = processorChain.get<masterGainIndex>();
        masterGain.setGainLinear (0.7f);
        auto& filter = processorChain.get<filterIndex>();            // get a reference to the filter with processorChain.get<>()
        filter.setCutoffFrequencyHz (1e3f);                          // set the cutoff frequency of the filter to 1 kHz
        filter.setResonance (0.7f);                                  // set the resonance of the filter to 0.7
        lfo.initialise ([] (float x) { return std::sin (x); }, 128); // initialise the lfo with a sine wave
        lfo.setFrequency (3.0f);                                     // change the lfo frequency to 3 Hz
    }
    //
    //==============================================================================
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        tempBlock = juce::dsp::AudioBlock<float> (heapBlock, spec.numChannels, spec.maximumBlockSize);
        processorChain.prepare (spec);
        lfo.prepare ({ spec.sampleRate / lfoUpdateRate, spec.maximumBlockSize, spec.numChannels }); // set the sample rate of the lfo to spec.sampleRate / lfoUpdateRate
    }
    //
    //==============================================================================
    void noteStarted() override
    {
        const auto velocity = getCurrentlyPlayingNote().noteOnVelocity.asUnsignedFloat();
        const auto freqHz = (float) getCurrentlyPlayingNote().getFrequencyInHertz();
        processorChain.get<osc1Index>().setFrequency (freqHz, true);
        processorChain.get<osc1Index>().setLevel (velocity);
        processorChain.get<osc2Index>().setFrequency (1.01f * freqHz, true); // set the frequency of the second oscillator to 1.01 * freqHz
        processorChain.get<osc2Index>().setLevel (velocity);                 // set the level of the second oscillator
    }
    //
    //==============================================================================
    void notePitchbendChanged() override
    {
        const auto freqHz = (float) getCurrentlyPlayingNote().getFrequencyInHertz();
        processorChain.get<osc1Index>().setFrequency (freqHz);
        processorChain.get<osc2Index>().setFrequency (1.01f * freqHz); // <- 3.5. set the frequency of the second oscillator to 1.01 * freqHz
    }
    //
    //==============================================================================
    void noteStopped (bool /*allowTailOff*/) override
    {
        clearCurrentNote();
    }
    //
    //==============================================================================
    void notePressureChanged() override {}
    void noteTimbreChanged() override   {}
    void noteKeyStateChanged() override {}
    //
    //==============================================================================
    void renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        for (int i = 0; i < numSamples; ++i)
        {
            if (--lfoUpdateCounter == 0)
            {
                lfoUpdateCounter = lfoUpdateRate;
                const auto lfoOut = lfo.processSample (0.0f);
                const auto cutoffFreqHz = jmap (lfoOut, -1.0f, 1.0f, 1e2f, 2e3f);
                processorChain.get<filterIndex>().setCutoffFrequencyHz (cutoffFreqHz);
            }
        }
        // call processSample() on the lfo once per lfoUpdateRate input samples (use lfoUpdateCounter)
        // use the output of lfo.processSample() to change the cutoff frequency of the filter between 100 Hz and 2 kHz
        auto block = tempBlock.getSubBlock (0, (size_t) numSamples);
        block.clear();
        juce::dsp::ProcessContextReplacing<float> context (block);
        processorChain.process (context);
        juce::dsp::AudioBlock<float> (outputBuffer).getSubBlock ((size_t) startSample, (size_t) numSamples).add (tempBlock);
    }
    //
private:
    //==============================================================================
    juce::HeapBlock<char> heapBlock;
    juce::dsp::AudioBlock<float> tempBlock;
    //
    enum{osc1Index, osc2Index, filterIndex, masterGainIndex};
    juce::dsp::ProcessorChain<Oscillator<float>, Oscillator<float>, juce::dsp::LadderFilter<float>, juce::dsp::Gain<float> > processorChain;
    static constexpr size_t lfoUpdateRate = 100;
    size_t lfoUpdateCounter = lfoUpdateRate;
    juce::dsp::Oscillator<float> lfo;   // declare a juce::dsp::Oscillator named lfo
};