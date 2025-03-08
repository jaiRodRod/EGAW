/*
  ==============================================================================

    MuteButton.h
    Created: 27 Feb 2025 11:35:58am
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SignalManagerUI.h"

//==============================================================================
/*
*/
class MuteButton : public juce::ToggleButton, public juce::Value::Listener
{
public:

    MuteButton(juce::ValueTree&);
    ~MuteButton() override;

    //void paint (juce::Graphics&) override;
    //void resized() override;

    void valueChanged(juce::Value& value) override; 

private:

    void setMute(bool);

    juce::ValueTree channelSettings;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MuteButton)
};
