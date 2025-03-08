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

//==============================================================================
/*
*/
class ChannelNameLabel  : public juce::Label
{
public:
    ChannelNameLabel(juce::ValueTree& channelSettings);
    ~ChannelNameLabel() override;

    //void paint (juce::Graphics&) override;
    //void resized() override;

    void valueChanged(juce::Value& value) override;

private:

    class InternalValueListener : public juce::Value::Listener
    {
    public:
        InternalValueListener(ChannelNameLabel& parent) : parentLabel(parent) {}

        void valueChanged(juce::Value& value) override
        {
            parentLabel.valueChanged(value);
        }

    private:
        ChannelNameLabel& parentLabel;
    };

    InternalValueListener valueListener{ *this };

    void textChange();

    juce::ValueTree channelSettings;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChannelNameLabel)
};
