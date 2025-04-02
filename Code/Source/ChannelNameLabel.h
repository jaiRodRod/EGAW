/*
  ==============================================================================

    ChannelNameLabel.h
    Created: 7 Mar 2025 9:45:06am
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SignalManagerUI.h"

class ChannelNameLabel : public juce::Label,
    public juce::MessageListener  // Changed from Value::Listener
{
public:
    ChannelNameLabel(juce::ValueTree& channelSettings);
    ~ChannelNameLabel() override;

    void handleMessage(const juce::Message& msg) override;  // Replaces valueChanged

private:
    void textChange();

    juce::ValueTree channelSettings;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChannelNameLabel)
};
