/*
  ==============================================================================

    RoutingOverlay.h
    Created: 25 Feb 2025 5:14:07pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RoutingActionStateManager.h"

//==============================================================================
/*
*/
class RoutingOverlay  : public juce::Component
{
public:
    RoutingOverlay();
    ~RoutingOverlay() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void mouseDown(const juce::MouseEvent& event) override;

private:

    juce::Component* recursiveFindRouteInButton(juce::Component*, juce::Point<int>);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RoutingOverlay)
};
