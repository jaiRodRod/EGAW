/*
  ==============================================================================

    RoutingActionStateManager.cpp
    Created: 24 Feb 2025 5:56:44pm
    Author:  jaezro

  ==============================================================================
*/

#include "RoutingActionStateManager.h"

JUCE_IMPLEMENT_SINGLETON(RoutingActionStateManager);

RoutingActionStateManager::RoutingActionStateManager()
{
    state = static_cast<int>(RoutingState::ROUTING_OFF);
}

RoutingActionStateManager::RoutingState RoutingActionStateManager::getCurrentState() const
{
    return static_cast<RoutingState>(static_cast<int>(state.getValue()));
}

void RoutingActionStateManager::setState(RoutingState newState)
{
    state = static_cast<int>(newState);
}

void RoutingActionStateManager::addListener(juce::Value::Listener* listener)
{
    state.addListener(listener);
}

void RoutingActionStateManager::removeListener(juce::Value::Listener* listener)
{
    state.removeListener(listener);
}

juce::String RoutingActionStateManager::getOriginChannelUuid()
{
    return originChannelUuid;
}

void RoutingActionStateManager::setOriginChannelUuid(juce::String originChannelUuid)
{
    this->originChannelUuid = originChannelUuid;
}

juce::String RoutingActionStateManager::getDestinyChannelUuid()
{
    return destinyChannelUuid;
}

void RoutingActionStateManager::setDestinyChannelUuid(juce::String destinyChannelUuid)
{
    this->destinyChannelUuid = destinyChannelUuid;
}

void RoutingActionStateManager::routingOff()
{
    setState(RoutingState::ROUTING_OFF);
    setOriginChannelUuid(juce::String());
    setDestinyChannelUuid(juce::String());
}
