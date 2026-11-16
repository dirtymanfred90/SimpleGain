#include "PluginEditor.h"

SimpleGainAudioProcessorEditor::SimpleGainAudioProcessorEditor(SimpleGainAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    gainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 120, 32);
    gainSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xff58d6c0));
    gainSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff424957));
    gainSlider.setColour(juce::Slider::thumbColourId, juce::Colours::white);
    gainSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    gainSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    gainSlider.setDoubleClickReturnValue(true, 0.0);
    gainSlider.textFromValueFunction = [](double value) { return juce::String(value, 2) + " dB"; };
    gainSlider.valueFromTextFunction = [](const juce::String& text) { return text.getFloatValue(); };
    addAndMakeVisible(gainSlider);
    gainAttachment = std::make_unique<SliderAttachment>(processor.parameters,
                                                         SimpleGainAudioProcessor::gainParameterId, gainSlider);

    gainLabel.setText("GAIN", juce::dontSendNotification);
    gainLabel.setJustificationType(juce::Justification::centred);
    gainLabel.setColour(juce::Label::textColourId, juce::Colour(0xffbfc8d5));
    addAndMakeVisible(gainLabel);
    setSize(340, 310);
}

void SimpleGainAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff171b24));
    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(23.0f, juce::Font::bold));
    g.drawFittedText("SIMPLE GAIN", 0, 18, getWidth(), 36, juce::Justification::centred, 1);
    g.setColour(juce::Colour(0xff667184));
    g.drawHorizontalLine(62, 28.0f, static_cast<float>(getWidth() - 28));
}

void SimpleGainAudioProcessorEditor::resized()
{
    gainSlider.setBounds(70, 75, 200, 190);
    gainLabel.setBounds(90, 267, 160, 24);
}
