#include "PluginProcessor.h"
#include "PluginEditor.h"

SimpleGainAudioProcessor::SimpleGainAudioProcessor()
    : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
                                     .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "PARAMETERS", createParameterLayout())
{
    gainParameter = parameters.getRawParameterValue(gainParameterId);
}

juce::AudioProcessorValueTreeState::ParameterLayout SimpleGainAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { gainParameterId, 1 }, "Gain",
        juce::NormalisableRange<float>(-60.0f, 12.0f, 0.01f), 0.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB")));
    return layout;
}

void SimpleGainAudioProcessor::prepareToPlay(double sampleRate, int)
{
    smoothedGain.reset(sampleRate, 0.025);
    smoothedGain.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(gainParameter->load()));
}

bool SimpleGainAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto& output = layouts.getMainOutputChannelSet();
    return (output == juce::AudioChannelSet::mono() || output == juce::AudioChannelSet::stereo())
           && output == layouts.getMainInputChannelSet();
}

void SimpleGainAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    const auto inputChannels = getTotalNumInputChannels();
    for (int channel = inputChannels; channel < getTotalNumOutputChannels(); ++channel)
        buffer.clear(channel, 0, buffer.getNumSamples());

    smoothedGain.setTargetValue(juce::Decibels::decibelsToGain(gainParameter->load()));
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        const float gain = smoothedGain.getNextValue();
        for (int channel = 0; channel < juce::jmin(inputChannels, buffer.getNumChannels()); ++channel)
            buffer.getWritePointer(channel)[sample] *= gain;
    }
}

juce::AudioProcessorEditor* SimpleGainAudioProcessor::createEditor()
{
    return new SimpleGainAudioProcessorEditor(*this);
}

void SimpleGainAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    if (auto xml = parameters.copyState().createXml())
        copyXmlToBinary(*xml, destData);
}

void SimpleGainAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary(data, sizeInBytes))
        if (xml->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleGainAudioProcessor();
}
