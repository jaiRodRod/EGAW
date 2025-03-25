/*
  ==============================================================================

    TimeViewer.h
    Created: 16 Mar 2025 8:03:19pm
    Author:  admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SharedLock_UI_Timers.h"

#include "GlobalPlayhead.h"

//==============================================================================
/*
*/
class TimeViewer  : public juce::Component, public juce::Timer
{
public:
    TimeViewer();
    ~TimeViewer() override;

    void timerCallback() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::Label timeLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimeViewer)
};
