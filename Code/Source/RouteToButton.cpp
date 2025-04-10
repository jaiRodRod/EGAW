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
    onClick = [this] {doRouteTo();};
    onRightClick = [this] { doRemoveRouteTo(); };
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

void RouteToButton::doRouteTo()
{
    RoutingActionStateManager::getInstance().setOriginChannelUuid(channelUuid);
    RoutingActionStateManager::getInstance().setState(RoutingActionStateManager::RoutingState::ROUTING_ON);
}

void RouteToButton::doRemoveRouteTo()
{
    RoutingActionStateManager::getInstance().setOriginChannelUuid(channelUuid);
    RoutingActionStateManager::getInstance().setState(RoutingActionStateManager::RoutingState::REMOVING_ROUTE);
}