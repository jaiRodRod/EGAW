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

//==============================================================================
/*
*/
class RouteToButton : public juce::TextButton
{
public:

    //using juce::TextButton::TextButton;

    RouteToButton(juce::String);
    //~RouteToButton() override;

    //void paint (juce::Graphics&) override;
    //void resized() override;

    void mouseUp(const juce::MouseEvent&) override;

    std::function<void()> onRightClick;

private:

    void doRouteTo();
    void doRemoveRouteTo();

    juce::String channelUuid;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RouteToButton)
};
