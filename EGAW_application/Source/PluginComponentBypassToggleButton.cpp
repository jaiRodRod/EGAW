/*
  ==============================================================================

    PluginComponentBypassToggleButton.cpp
    Created: 23 Apr 2025 7:23:13pm
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PluginComponentBypassToggleButton.h"

//==============================================================================
PluginComponentBypassToggleButton::PluginComponentBypassToggleButton(juce::ValueTree& pluginSettings) : juce::ToggleButton(), pluginSettings(pluginSettings)
{
    SignalManagerUI::getInstance().addListener(this);

    svgDrawable = juce::Drawable::createFromImageData(BinaryData::bypass_svg, BinaryData::bypass_svgSize);

    setToggleable(true);
    setToggleState(pluginSettings.getProperty("isBypassed"), false);
    setClickingTogglesState(true);
    onClick = [this] { setBypassed(getToggleState()); };

    setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    setMouseCursor(juce::MouseCursor::PointingHandCursor);

	setTooltip("Bypass");
}

PluginComponentBypassToggleButton::~PluginComponentBypassToggleButton()
{
    SignalManagerUI::getInstance().removeListener(this);
}

void PluginComponentBypassToggleButton::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    bool isOn = pluginSettings.getProperty("isBypassed");
    bool isOver = isMouseOver();

    // Calculate colors based on state
    juce::Colour bgColour;
    if (isOn) {
        bgColour = colourConstants::light_blue; // Active state - full yellow
    }
    else if (isOver) {
        bgColour = colourConstants::light_blue.darker(0.6f).brighter(0.3f); // Hover state - lighter yellow
    }
    else {
        bgColour = colourConstants::light_blue.darker(0.6f); // Normal state - light yellow
    }

    // Draw button background
    g.setColour(bgColour);
    g.fillRoundedRectangle(bounds, 2.0f);

    // Draw outline
    g.setColour(juce::Colours::black.withAlpha(0.2f));
    g.drawRoundedRectangle(bounds, 2.0f, 1.0f);

    // Draw text
    /*
    g.setColour(findColour(isOn ? juce::TextButton::textColourOnId :
        juce::TextButton::textColourOffId));
    g.setFont(buttonFont);
    g.drawText(getButtonText(),
        bounds.reduced(padding),
        juce::Justification::centred,
        false);
    */

    /*
    if (svgDrawable != nullptr)
    {
        // Adjust SVG color based on state
        svgDrawable->replaceColour(
            juce::Colours::black,
            isOn ? juce::Colours::black : juce::Colours::black.withAlpha(0.8f)
        );

        svgDrawable->setTransformToFit(bounds.reduced(4), juce::RectanglePlacement::centred);
        //svgDrawable->setTransform(juce::AffineTransform::rotation(juce::degreesToRadians(90.f)));
        svgDrawable->draw(g, 1.0f);
    }
    */

    if (svgDrawable != nullptr)
    {
        // Adjust SVG color based on state
        svgDrawable->replaceColour(
            juce::Colours::black,
            isOn ? juce::Colours::black : juce::Colours::black.withAlpha(0.8f)
        );

        // Create a transform that rotates around the center
        auto transform = juce::AffineTransform::rotation(juce::degreesToRadians(90.0f),
            bounds.getCentreX(),
            bounds.getCentreY());

        // Set the transform and draw
        g.saveState();
        g.addTransform(transform);
        svgDrawable->setTransformToFit(bounds.reduced(4), juce::RectanglePlacement::centred);
        svgDrawable->draw(g, 1.0f);
        g.restoreState();
    }
}

void PluginComponentBypassToggleButton::handleMessage(const juce::Message& message)
{
    if (auto* signalMsg = dynamic_cast<const SignalMessage*>(&message)) {
        auto signal = static_cast<SignalManagerUI::Signal>(signalMsg->getSignalType());
        // Handle signal (already on message thread)...

        switch (signal)
        {
        case SignalManagerUI::Signal::RESTORE_UI_PARAMETERS:
            setToggleState(pluginSettings.getProperty("isBypassed"), false);
            break;
        default:
            break;
        }
    }
}

void PluginComponentBypassToggleButton::setBypassed(bool bypassButtonToggleState)
{
	pluginSettings.setProperty("isBypassed", bypassButtonToggleState, nullptr);
}
