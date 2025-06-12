/*
  ==============================================================================

    RouteInButton.cpp
    Created: 26 Feb 2025 11:05:14am
    Author:  jaezro

  ==============================================================================
*/

#include "RouteInButton.h"

RouteInButton::RouteInButton(juce::String channelUuid) : juce::TextButton(), channelUuid(channelUuid)
{
	setTooltip("When routing left click to route / right click to remove route");
    onClick = [this] {routeInClicked();};
}

void RouteInButton::paint(juce::Graphics& g)
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

juce::Point<int> RouteInButton::getComponentCenter()
{
	return getLocalBounds().getCentre();
}

void RouteInButton::routeInClicked()
{
    auto state = RoutingActionStateManager::getInstance().getCurrentState();

    switch (state)
    {
    case RoutingActionStateManager::RoutingState::ROUTING_ON:
        doRouteIn();
        break;
    case RoutingActionStateManager::RoutingState::REMOVING_ROUTE:
        doRemoveRouteIn();
        break;
    default:
        break;
    }
}

void RouteInButton::doRouteIn()
{
    //DBG("Routing!!!");
    RoutingActionStateManager::getInstance().setDestinyChannelUuid(channelUuid);
    RoutingActionStateManager::getInstance().setState(RoutingActionStateManager::RoutingState::COMPLETED_ROUTING);
}

void RouteInButton::doRemoveRouteIn()
{
    //DBG("Removing Route!!!");
    RoutingActionStateManager::getInstance().setDestinyChannelUuid(channelUuid);
    RoutingActionStateManager::getInstance().setState(RoutingActionStateManager::RoutingState::COMPLETED_REMOVING_ROUTE);
}

void RouteInButton::drawHexagon(juce::Graphics& g, float centerX, float centerY, float size)
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