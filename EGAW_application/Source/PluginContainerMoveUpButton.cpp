/*
  ==============================================================================

    PluginContainerMoveUpButton.cpp
    Created: 23 Apr 2025 7:22:44pm
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PluginContainerMoveUpButton.h"

//==============================================================================
PluginContainerMoveUpButton::PluginContainerMoveUpButton(juce::ValueTree& pluginState, juce::String pluginUuid) : pluginState(pluginState), pluginUuid(pluginUuid)
{
	setMouseCursor(juce::MouseCursor::PointingHandCursor);
	setTooltip("Click to move plugin up");
	onClick = [this] { moveUpAction(); };
}

PluginContainerMoveUpButton::~PluginContainerMoveUpButton()
{
}

void PluginContainerMoveUpButton::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    bool isOver = isMouseOver();
    bool isDown = isMouseButtonDown();

    // Calculate triangle size (80% of smallest dimension)
    float size = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.8f;

    // Create equilateral triangle path pointing upwards
    juce::Path triangle;
    triangle.addTriangle(bounds.getCentreX(), bounds.getCentreY() - size / 2,  // Top point
        bounds.getCentreX() - size / 2, bounds.getCentreY() + size / 2,  // Bottom left
        bounds.getCentreX() + size / 2, bounds.getCentreY() + size / 2); // Bottom right

    // Set color with hover/down states
    juce::Colour triangleColor = juce::Colours::black;
    if (isDown) {
        triangleColor = juce::Colours::black.withAlpha(0.7f); // Slightly transparent when pressed
    }
    else if (isOver) {
        triangleColor = juce::Colours::black.brighter(0.2f); // Brighter when hovered
    }

    g.setColour(triangleColor);
    g.fillPath(triangle);
}

void PluginContainerMoveUpButton::resized()
{
}

void PluginContainerMoveUpButton::moveUpAction()
{
    auto pluginIndex = pluginState.getChildWithName("pluginOrder").indexOf(pluginState.getChildWithName("pluginOrder").getChildWithName(pluginUuid));
    if (pluginIndex > 0)
    {
        pluginState.getChildWithName("pluginOrder").moveChild(pluginIndex, pluginIndex - 1, nullptr);
    }
}

