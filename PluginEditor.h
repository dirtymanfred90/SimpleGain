#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class GlowKnobLookAndFeel final : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider(juce::Graphics&, int, int, int, int, float,
                          float, float, juce::Slider&) override;
};

class SimpleGainAudioProcessorEditor final : public juce::AudioProcessorEditor,
                                              private juce::Timer
{
public:
    explicit SimpleGainAudioProcessorEditor(SimpleGainAudioProcessor&);
    ~SimpleGainAudioProcessorEditor() override;
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    void timerCallback() override;
    SimpleGainAudioProcessor& processor;
    GlowKnobLookAndFeel knobLook;
    juce::Slider gainSlider;
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> gainAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleGainAudioProcessorEditor)
};
