/*
  ==============================================================================

    RouteToButton.cpp
    Created: 27 Feb 2025 11:35:17am
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RouteToButton.h"

//==============================================================================
RouteToButton::RouteToButton(juce::String channelUuid) : juce::TextButton(), channelUuid(channelUuid)
{
	setTooltip("Right click to route in / Left click to remove route");
	setMouseCursor(juce::MouseCursor::PointingHandCursor);
    onClick = [this] {doRouteTo();};
    onRightClick = [this] { doRemoveRouteTo(); };
}

void RouteToButton::paint(juce::Graphics& g)
{
    auto externalDiameter = getLocalBounds().getHeight() - 10;
	auto externalRadius = externalDiameter * 0.5f;

    g.setColour(juce::Colours::grey);
    g.fillEllipse(getLocalBounds().getCentreX() - externalRadius, getLocalBounds().getCentreY() - externalRadius, externalDiameter, externalDiameter);
    g.setColour(juce::Colours::black);
    g.drawEllipse(getLocalBounds().getCentreX() - externalRadius, getLocalBounds().getCentreY() - externalRadius, externalDiameter, externalDiameter, 2.0f);

	//g.setColour(juce::Colours::lightgrey);
	auto hexFactor = 0.8f; // Adjust this factor to change the size of the hexagon
	drawHexagon(g, getLocalBounds().getCentreX(), getLocalBounds().getCentreY(), externalRadius * hexFactor);

	auto plugFactor = 0.5f; // Adjust this factor to change the size of the plug
    g.setColour(juce::Colours::black.brighter(0.2f));
    g.fillEllipse(getLocalBounds().getCentreX() - (externalRadius * plugFactor), getLocalBounds().getCentreY() - (externalRadius * plugFactor), externalDiameter * plugFactor, externalDiameter * plugFactor);
}

void RouteToButton::mouseUp(const juce::MouseEvent& event)
{
    if (event.mods.isRightButtonDown())
    {
        if (isEnabled() && contains(event.getPosition()))
            if (onRightClick)
                onRightClick();
    }
    else
    {
        TextButton::mouseUp(event);
    }
}

juce::Point<int> RouteToButton::getComponentCenter()
{
	return getLocalBounds().getCentre();
}

void RouteToButton::doRouteTo()
{
    RoutingActionStateManager::getInstance().setOriginChannelUuid(channelUuid);
    RoutingActionStateManager::getInstance().setState(RoutingActionStateManager::RoutingState::ROUTING_ON);

	ChannelRouteViewSingleton::getInstance().setChannelUuid(channelUuid);
	ChannelRouteViewSingleton::getInstance().setState(ChannelRouteViewSingleton::RouteViewState::VIEWING_ON);
}

void RouteToButton::doRemoveRouteTo()
{
    RoutingActionStateManager::getInstance().setOriginChannelUuid(channelUuid);
    RoutingActionStateManager::getInstance().setState(RoutingActionStateManager::RoutingState::REMOVING_ROUTE);

    ChannelRouteViewSingleton::getInstance().setChannelUuid(channelUuid);
    ChannelRouteViewSingleton::getInstance().setState(ChannelRouteViewSingleton::RouteViewState::VIEWING_ON);
}

void RouteToButton::drawHexagon(juce::Graphics& g, float centerX, float centerY, float size)
{
    juce::Path hexagonPath;

    // Calculate the 6 points of the hexagon
    for (int i = 0; i < 6; ++i)
    {
        float angle = juce::MathConstants<float>::twoPi * i / 6.0f;
        float x = centerX + size * std::cos(angle);
        float y = centerY + size * std::sin(angle);

        if (i == 0)
            hexagonPath.startNewSubPath(x, y);
        else
            hexagonPath.lineTo(x, y);
    }

    hexagonPath.closeSubPath();

    // Draw the hexagon
    g.setColour(juce::Colours::lightgrey.darker(0.4f));
    g.fillPath(hexagonPath);

    // Optional outline
    g.setColour(juce::Colours::black);
    g.strokePath(hexagonPath, juce::PathStrokeType(2.0f));
}