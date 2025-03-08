/*
  ==============================================================================

    RoutableChannel.cpp
    Created: 9 Feb 2025 11:50:49am
    Author:  jaezro

  ==============================================================================
*/

#include "RoutableChannel.h"

RoutableChannel::RoutableChannel()
{

}

RoutableChannel::RoutableChannel(juce::String& optionalInternalChannelId) : Channel(optionalInternalChannelId)
{

}

RoutableChannel::~RoutableChannel()
{

}

void RoutableChannel::routeIn(Channel* channelIn)
{
    channelsRoutedIn.addIfNotAlreadyThere(channelIn);
}

void RoutableChannel::removeRouteIn(Channel* channelIn)
{
    channelsRoutedIn.removeFirstMatchingValue(channelIn);
}

juce::Array<Channel*> RoutableChannel::getChanelsRoutedIn() const
{
    return channelsRoutedIn;
}