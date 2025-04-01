/*
  ==============================================================================

    TimeViewer.h
    Created: 16 Mar 2025 8:03:19pm
    Author:  admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "GlobalPlayhead.h"

//==============================================================================
/*
*/
class TimeViewer  : public juce::Component, public juce::ValueTree::Listener//, public juce::Timer
{
public:
    TimeViewer(juce::ValueTree&);
    ~TimeViewer() override;

    //void timerCallback() override;
    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::ValueTree playheadState;
    juce::Label timeLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimeViewer)
};
