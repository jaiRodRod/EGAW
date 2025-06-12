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
#include "SignalManagerUI.h"

class RouteInButton : public juce::TextButton
{
public:

    RouteInButton(juce::String);
	~RouteInButton() override = default;

	void paint(juce::Graphics&) override;

    juce::Point<int> getComponentCenter();

private:

    void routeInClicked();
    void doRouteIn();
    void doRemoveRouteIn();

    void drawHexagon(juce::Graphics& g, float centerX, float centerY, float size);

    juce::String channelUuid;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RouteInButton);
};