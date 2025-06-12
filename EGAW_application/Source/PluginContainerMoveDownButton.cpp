/*
  ==============================================================================

    PluginContainerMoveDownButton.cpp
    Created: 23 Apr 2025 7:22:58pm
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PluginContainerMoveDownButton.h"

//==============================================================================
PluginContainerMoveDownButton::PluginContainerMoveDownButton(juce::ValueTree& pluginState, juce::String pluginUuid) : pluginState(pluginState), pluginUuid(pluginUuid)
{
	setMouseCursor(juce::MouseCursor::PointingHandCursor);
	setTooltip("Click to move plugin down");
    onClick = [this] { moveDownAction(); };
}

PluginContainerMoveDownButton::~PluginContainerMoveDownButton()
{
}

void PluginContainerMoveDownButton::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    bool isOver = isMouseOver();
    bool isDown = isMouseButtonDown();

    // Calculate triangle size (80% of smallest dimension)
    float size = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.8f;

    // Create inverted equilateral triangle path (pointing downward)
    juce::Path triangle;
    triangle.addTriangle(bounds.getCentreX(), bounds.getCentreY() + size / 2,  // Bottom point
        bounds.getCentreX() - size / 2, bounds.getCentreY() - size / 2,  // Top left
        bounds.getCentreX() + size / 2, bounds.getCentreY() - size / 2); // Top right

    // Set color with hover/down states
    juce::Colour triangleColor = juce::Colours::black;
    if (isDown) {
        triangleColor = juce::Colours::black.withAlpha(0.7f);
    }
    else if (isOver) {
        triangleColor = juce::Colours::black.brighter(0.2f);
    }

    g.setColour(triangleColor);
    g.fillPath(triangle);
}

void PluginContainerMoveDownButton::resized()
{
}

void PluginContainerMoveDownButton::moveDownAction()
{
    auto pluginIndex = pluginState.getChildWithName("pluginOrder").indexOf(pluginState.getChildWithName("pluginOrder").getChildWithName(pluginUuid));
    if (pluginIndex < 5)
    {
        pluginState.getChildWithName("pluginOrder").moveChild(pluginIndex, pluginIndex + 1, nullptr);
    }
}
