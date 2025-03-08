/*
  ==============================================================================

    RoutableChannel.h
    Created: 9 Feb 2025 11:50:49am
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "Channel.h"

class RoutableChannel : public Channel
{
public:
    RoutableChannel();
    RoutableChannel(juce::String&);
    ~RoutableChannel();

    virtual void routeIn(Channel*);
    virtual void removeRouteIn(Channel*);
    juce::Array<Channel*> getChanelsRoutedIn() const;

private:
    juce::Array<Channel*> channelsRoutedIn;
};