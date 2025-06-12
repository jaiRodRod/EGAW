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

#include "ColourConstants.h"

//==============================================================================
/*
*/
class MuteButton : public juce::ToggleButton, public juce::MessageListener
{
public:

    MuteButton(juce::ValueTree&);
    ~MuteButton() override;

    void paint (juce::Graphics& g) override;
    void resized() override;
	void handleMessage(const juce::Message& message) override;

private:

    void setMute(bool);
    void updateFontSize();

    juce::ValueTree channelSettings;
    juce::Font buttonFont{ 14.0f };
    const int padding = 4; // Space between text and button edges

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MuteButton)
};
