/*
  ==============================================================================

    RouteInButton.h
    Created: 26 Feb 2025 11:05:14am
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "RoutingActionStateManager.h"

class RouteInButton : public juce::TextButton
{
public:

    RouteInButton(juce::String);

private:

    void routeInClicked();
    void doRouteIn();
    void doRemoveRouteIn();

    juce::String channelUuid;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RouteInButton);
};