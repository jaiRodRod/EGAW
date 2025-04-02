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
    onClick = [this] {routeInClicked();};
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
    DBG("Routing!!!");
    RoutingActionStateManager::getInstance().setDestinyChannelUuid(channelUuid);
    RoutingActionStateManager::getInstance().setState(RoutingActionStateManager::RoutingState::COMPLETED_ROUTING);
}

void RouteInButton::doRemoveRouteIn()
{
    DBG("Removing Route!!!");
    RoutingActionStateManager::getInstance().setDestinyChannelUuid(channelUuid);
    RoutingActionStateManager::getInstance().setState(RoutingActionStateManager::RoutingState::COMPLETED_REMOVING_ROUTE);
}
