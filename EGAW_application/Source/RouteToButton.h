/*
  ==============================================================================

    RouteToButton.h
    Created: 27 Feb 2025 11:35:17am
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "RoutingActionStateManager.h"
#include "ChannelRouteViewSingleton.h"

//==============================================================================
/*
*/
class RouteToButton : public juce::TextButton
{
public:

    //using juce::TextButton::TextButton;

    RouteToButton(juce::String);
    //~RouteToButton() override;

    void paint (juce::Graphics&) override;
    //void resized() override;

    void mouseUp(const juce::MouseEvent&) override;

    juce::Point<int> getComponentCenter();
    
    std::function<void()> onRightClick;

private:

    void doRouteTo();
    void doRemoveRouteTo();

    void drawHexagon(juce::Graphics& g, float centerX, float centerY, float size);

    juce::String channelUuid;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RouteToButton)
};
