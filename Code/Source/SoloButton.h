/*
  ==============================================================================

    SoloButton.h
    Created: 4 Mar 2025 9:36:52am
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SignalManagerUI.h"

//==============================================================================
/*
*/
class SoloButton  : public juce::ToggleButton, public juce::Value::Listener
{
public:
    SoloButton(juce::ValueTree&);
    ~SoloButton() override;

    //void paint (juce::Graphics&) override;
    //void resized() override;

    void valueChanged(juce::Value& value) override;

private:

    void setSolo(bool);

    juce::ValueTree channelSettings;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoloButton)
};
