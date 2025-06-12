/*
  ==============================================================================

    MuteButton.cpp
    Created: 27 Feb 2025 11:35:58am
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MuteButton.h"

//==============================================================================
MuteButton::MuteButton(juce::ValueTree& channelSettings) : juce::ToggleButton(), channelSettings(channelSettings)
{
    SignalManagerUI::getInstance().addListener(this);

	setTooltip("Click to mute/unmute channel");

    setToggleable(true);
    setToggleState(channelSettings.getProperty("Mute"), false);
    setClickingTogglesState(true);
    onClick = [this] { setMute(getToggleState()); };

    setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
}

MuteButton::~MuteButton()
{
    SignalManagerUI::getInstance().removeListener(this);
}

void MuteButton::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    bool isOn = channelSettings.getProperty("Mute");
    bool isOver = isMouseOver();

    // Calculate colors based on state
    juce::Colour bgColour;
    if (isOn) {
        bgColour = colourConstants::light_red; // Active state - full yellow
    }
    else if (isOver) {
        bgColour = colourConstants::plain_red.brighter(0.3f); // Hover state - lighter yellow
    }
    else {
        bgColour = colourConstants::plain_red; // Normal state - light yellow
    }

    // Draw button background
    g.setColour(bgColour);
    g.fillRoundedRectangle(bounds, 2.0f);

    // Draw outline
    g.setColour(juce::Colours::black.withAlpha(0.2f));
    g.drawRoundedRectangle(bounds, 2.0f, 1.0f);

    // Draw text
    g.setColour(findColour(isOn ? juce::TextButton::textColourOnId :
        juce::TextButton::textColourOffId));
    g.setFont(buttonFont);
    g.drawText(getButtonText(),
        bounds.reduced(padding),
        juce::Justification::centred,
        false);
}

void MuteButton::resized()
{
    updateFontSize();
}

void MuteButton::handleMessage(const juce::Message& message)
{
    if (auto* signalMsg = dynamic_cast<const SignalMessage*>(&message)) {
        auto signal = static_cast<SignalManagerUI::Signal>(signalMsg->getSignalType());
        // Handle signal (already on message thread)...

        switch (signal)
        {
        case SignalManagerUI::Signal::RESTORE_UI_PARAMETERS:
            setToggleState(channelSettings.getProperty("Mute"), false);
            break;
        default:
            break;
        }
    }
}

void MuteButton::setMute(bool muteButtonToggleState)
{
    channelSettings.setProperty("Mute", muteButtonToggleState, nullptr);
}

void MuteButton::updateFontSize()
{
    // Calculate font size based on button width and text length
    int width = getWidth() - (2 * padding);
    int height = getHeight() - (2 * padding);

    // Start with height-based size (80% of available height)
    float fontSize = height * 0.8f;

    // Adjust if text would be too wide
    if (getButtonText().isNotEmpty()) {
        float textWidth = buttonFont.getStringWidth(getButtonText());
        if (textWidth > width) {
            fontSize *= width / textWidth;
        }
    }

    // Set minimum and maximum font sizes
    fontSize = juce::jlimit(8.0f, 24.0f, fontSize);
    buttonFont = juce::Font(fontSize * 0.8f, juce::Font::bold);
}
