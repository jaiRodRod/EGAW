/*
  ==============================================================================

    TransportBoxComponent.h
    Created: 22 Mar 2025 11:36:09am
    Author:  admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ZoomConstants.h"
#include "SharedLock_UI_Timers.h"

#include "TransportComponent.h"
#include "GlobalPlayhead.h"

//==============================================================================
/*
*/
class TransportBoxComponent  : public juce::Component, public juce::Timer
{
public:
    TransportBoxComponent(juce::ValueTree&);
    ~TransportBoxComponent() override;

    void timerCallback() override;
    void mouseDoubleClick (const juce::MouseEvent& event) override;
    void mouseDrag (const juce::MouseEvent& event) override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::ValueTree projectData;

    int transportViewPosition;
    TransportComponent transportComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TransportBoxComponent)
};
