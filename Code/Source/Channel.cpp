/*
  ==============================================================================

    Channel.cpp
    Created: 9 Feb 2025 11:50:31am
    Author:  jaezro

  ==============================================================================
*/

#include "Channel.h"

#include "RoutableChannel.h"

Channel::Channel() : internalChannelId(), channelsRoutedTo("channelsRoutedTo")
{
}

Channel::Channel(juce::String& optionalInternalChannelId) : internalChannelId(optionalInternalChannelId), channelsRoutedTo("channelsRoutedTo")
{
}

Channel::~Channel()
{

}

void Channel::routeTo(RoutableChannel* channelToRoute, bool isNewRoute = false)
{
    if (channelsRoutedTo.getChildWithName(channelToRoute->getInternalChannelId()) == juce::ValueTree{})
    {
        if (isNewRoute)
        {
            juce::ValueTree channelToRouteValueTree(channelToRoute->getInternalChannelId());
            channelsRoutedTo.appendChild(channelToRouteValueTree, nullptr);
        }
        channelToRoute->routeIn(this);
    }
}

void Channel::removeRouteTo(RoutableChannel* channelToRemoveRoute)
{
    auto channelToRemoveRouteValueTree = channelsRoutedTo.getChildWithName(channelToRemoveRoute->getInternalChannelId());
    if (channelToRemoveRouteValueTree != juce::ValueTree{})
    {
        channelToRemoveRoute->removeRouteIn(this);
        channelsRoutedTo.removeChild(channelToRemoveRouteValueTree, nullptr);
    }
}

juce::String Channel::getInternalChannelId()
{
    //juce::Logger::writeToLog(internalChannelId.toString());
    return internalChannelId.toString();
}

void Channel::setInternalChannelId(juce::Uuid uuid)
{
    internalChannelId = uuid;
}

juce::ValueTree Channel::getValueTree()
{
    return channelsRoutedTo;
}

juce::Value Channel::getMixerPosition()
{
    return mixerPosition;
}

void Channel::setMixerPosition(int position)
{
    mixerPosition = position;
}