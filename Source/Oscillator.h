/*
  ==============================================================================

    Oscillator.h
    Created: 26 Nov 2017 10:15:21am
    Author:  John Carlson

  ==============================================================================
*/
//
#pragma once
//
template <typename Type>
class Oscillator
{
public:
    //==============================================================================
    Oscillator()
    {
        auto& osc = processorChain.template get<oscIndex>();        // get a reference to the Oscillator with processorChain.get<>()
        osc.initialise ([] (Type x) { return std::sin (x); }, 128); // initialise the Oscillator with a 128-point sine wave lookup table
        osc.initialise ([] (Type x) { return jmap (x, Type (-double_Pi), Type (double_Pi), Type (-1), Type (1)); }, 2); // <- 2.1. initialise the Oscillator with a 2-point sawtooth wave (i.e. a linear ramp from -1 to 1) lookup table
    }
    //
    //==============================================================================
    void setFrequency (Type newValue, bool force = false)
    {
        ignoreUnused (newValue);
        ignoreUnused (force);
        auto& osc = processorChain.template get<oscIndex>(); // get a reference to the Oscillator with processorChain.get<>()
        osc.setFrequency (newValue, force);                  // set the frequency of the Oscillator
    }
    //
    //==============================================================================
    void setLevel (Type newValue)
    {
        ignoreUnused (newValue);
        auto& gain = processorChain.template get<gainIndex>(); // get a reference to the Gain with processorChain.get<>()
        gain.setGainLinear (newValue);                         // set the gain of the Gain here
    }
    //
    //==============================================================================
    void reset() noexcept
    {
        processorChain.reset();
    }
    //
    //==============================================================================
    template <typename ProcessContext>
    void process (const ProcessContext& context) noexcept
    {
        auto&& outBlock = context.getOutputBlock();
        const auto numSamples = outBlock.getNumSamples();
        ignoreUnused (numSamples);
        auto blockToUse = tempBlock.getSubBlock (0, numSamples);                   // create an AudioBlock with the correct size (numSamples) from tempBlock using getSubBlock()
        auto contextToUse = juce::dsp::ProcessContextReplacing<Type> (blockToUse); // create a juce::dsp::ProcessContextReplacing from the AudioBlock above
        processorChain.process (contextToUse);                                     // process processorChain with the above context
        outBlock.add (blockToUse);                                                 // add the results to outBlock
    }
    //
    //==============================================================================
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        ignoreUnused (spec);
        processorChain.prepare (spec);// prepare() the processorChain
        tempBlock = juce::dsp::AudioBlock<Type> (heapBlock, spec.numChannels, spec.maximumBlockSize); // Create a new tempBlock here with spec.numChannels channels and spec.maximumBlockSize samples. Pass the HeapBlock declared below to the constructor, which will handle memory allocation.
    }
    //
private:
    //==============================================================================
    enum{oscIndex, gainIndex};
    //
    juce::dsp::ProcessorChain<juce::dsp::Oscillator<Type>, juce::dsp::Gain<Type>> processorChain;
    juce::HeapBlock<char> heapBlock;        // declare a juce::HeapBlock<char>
    juce::dsp::AudioBlock<Type> tempBlock;  // declare a juce::dsp::AudioBlock named tempBlock
};