#include "PluginEditor.h"
#include <cmath>

namespace
{
    const juce::Colour bg(0xff111820);
    const juce::Colour cyan(0xff39e7e2);
    const juce::Colour muted(0xff70838e);
}

void GlowKnobLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                           float pos, float startAngle, float endAngle,
                                           juce::Slider&)
{
    const float cx = x + width * 0.5f;
    const float cy = y + height * 0.5f;
    const float radius = juce::jmin(width, height) * 0.365f;
    const float angle = startAngle + pos * (endAngle - startAngle);
    const float intensity = std::pow(juce::jlimit(0.0f, 1.0f, pos), 1.35f);

    // Layered translucent rings provide a soft glow without external image assets.
    for (int i = 13; i >= 1; --i)
    {
        const float thickness = 1.6f + i * 2.2f;
        juce::Path arc;
        arc.addCentredArc(cx, cy, radius + 4.0f, radius + 4.0f, 0.0f,
                          startAngle, angle, true);
        g.setColour(cyan.withAlpha((0.003f + intensity * 0.010f) * (14 - i) / 13.0f));
        g.strokePath(arc, juce::PathStrokeType(thickness, juce::PathStrokeType::curved,
                                               juce::PathStrokeType::rounded));
    }

    juce::Path track;
    track.addCentredArc(cx, cy, radius + 4.0f, radius + 4.0f, 0.0f,
                        startAngle, endAngle, true);
    g.setColour(juce::Colour(0xff273a46));
    g.strokePath(track, juce::PathStrokeType(5.0f, juce::PathStrokeType::curved,
                                             juce::PathStrokeType::rounded));

    juce::Path active;
    active.addCentredArc(cx, cy, radius + 4.0f, radius + 4.0f, 0.0f,
                         startAngle, angle, true);
    g.setColour(cyan.withAlpha(0.28f + 0.72f * intensity));
    g.strokePath(active, juce::PathStrokeType(4.0f, juce::PathStrokeType::curved,
                                              juce::PathStrokeType::rounded));

    for (int i = 0; i <= 32; ++i)
    {
        const float a = startAngle + (endAngle - startAngle) * i / 32.0f;
        const float inner = radius + 15.0f;
        const float outer = inner + ((i % 4 == 0) ? 8.0f : 4.0f);
        g.setColour(i / 32.0f <= pos ? cyan.withAlpha(0.20f + intensity * 0.58f)
                                          : muted.withAlpha(0.36f));
        g.drawLine(cx + std::sin(a) * inner, cy - std::cos(a) * inner,
                   cx + std::sin(a) * outer, cy - std::cos(a) * outer, 1.2f);
    }

    auto body = juce::Rectangle<float>(cx - radius + 9.0f, cy - radius + 9.0f,
                                       (radius - 9.0f) * 2.0f, (radius - 9.0f) * 2.0f);
    g.setColour(juce::Colour(0xff34444e));
    g.fillEllipse(body.expanded(3.0f));
    g.setGradientFill(juce::ColourGradient(juce::Colour(0xff35434b), cx - radius, cy - radius,
                                           juce::Colour(0xff101820), cx + radius, cy + radius, false));
    g.fillEllipse(body);
    g.setColour(juce::Colour(0xff4d616a).withAlpha(0.75f));
    g.drawEllipse(body.reduced(4.0f), 1.0f);

    const float dotRadius = radius * 0.63f;
    const float px = cx + std::sin(angle) * dotRadius;
    const float py = cy - std::cos(angle) * dotRadius;
    g.setColour(cyan.withAlpha(0.22f + 0.55f * intensity));
    g.fillEllipse(px - 7.0f, py - 7.0f, 14.0f, 14.0f);
    g.setColour(cyan.brighter(0.35f));
    g.fillEllipse(px - 2.7f, py - 2.7f, 5.4f, 5.4f);
}

SimpleGainAudioProcessorEditor::SimpleGainAudioProcessorEditor(SimpleGainAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    gainSlider.setLookAndFeel(&knobLook);
    gainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainSlider.setRotaryParameters(juce::MathConstants<float>::pi * 1.23f,
                                   juce::MathConstants<float>::pi * 2.77f, true);
    gainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    gainSlider.setDoubleClickReturnValue(true, 0.0);
    gainSlider.setPopupDisplayEnabled(false, false, this);
    addAndMakeVisible(gainSlider);
    gainAttachment = std::make_unique<SliderAttachment>(processor.parameters,
                                                          SimpleGainAudioProcessor::gainParameterId, gainSlider);
    setSize(420, 420);
    startTimerHz(30);
}

SimpleGainAudioProcessorEditor::~SimpleGainAudioProcessorEditor()
{
    stopTimer();
    gainAttachment.reset();
    gainSlider.setLookAndFeel(nullptr);
}

void SimpleGainAudioProcessorEditor::timerCallback()
{
    // Automation changes update the arc and digital readout.
    repaint();
    gainSlider.repaint();
}

void SimpleGainAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(bg);
    auto bounds = getLocalBounds().toFloat().reduced(10.0f);
    g.setColour(juce::Colour(0xff1a2530));
    g.fillRoundedRectangle(bounds, 14.0f);
    g.setColour(juce::Colour(0xff304550));
    g.drawRoundedRectangle(bounds, 14.0f, 1.0f);

    g.setColour(cyan);
    g.fillRoundedRectangle(26.0f, 30.0f, 5.0f, 24.0f, 2.0f);
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(juce::FontOptions(21.0f, juce::Font::bold)));
    g.drawText("SIMPLE GAIN", 42, 26, 260, 34, juce::Justification::centredLeft);
    g.setColour(muted);
    g.setFont(juce::Font(juce::FontOptions(12.0f)));
    g.drawText("v2.0", 337, 30, 55, 26, juce::Justification::centredRight);
    g.setColour(juce::Colour(0xff304550));
    g.drawLine(25.0f, 69.0f, 395.0f, 69.0f, 1.0f);

    const double db = gainSlider.getValue();
    const auto readout = (db > 0.0001 ? "+" : "") + juce::String(db, 1) + " dB";
    g.setColour(cyan.withAlpha(0.60f + 0.40f * static_cast<float>(gainSlider.valueToProportionOfLength(db))));
    g.setFont(juce::Font(juce::FontOptions(31.0f, juce::Font::bold)));
    g.drawText(readout, 40, 317, 340, 40, juce::Justification::centred);
    g.setColour(muted);
    g.setFont(juce::Font(juce::FontOptions(12.0f)));
    g.drawText("GAIN", 0, 366, getWidth(), 22, juce::Justification::centred);
    g.drawText("-60 dB", 27, 299, 90, 18, juce::Justification::centredLeft);
    g.drawText("+12 dB", 303, 299, 90, 18, juce::Justification::centredRight);
}

void SimpleGainAudioProcessorEditor::resized()
{
    gainSlider.setBounds(67, 78, 286, 235);
}
