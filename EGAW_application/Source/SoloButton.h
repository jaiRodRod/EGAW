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

#include "ColourConstants.h"

//==============================================================================
/*
*/
class SoloButton  : public juce::ToggleButton, public juce::MessageListener
{
public:
    SoloButton(juce::ValueTree&);
    ~SoloButton() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
	void handleMessage(const juce::Message& message) override;

private:

    void setSolo(bool);
    void updateFontSize();

    juce::ValueTree channelSettings;
    juce::Font buttonFont{ 14.0f };
    const int padding = 4; // Space between text and button edges


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoloButton)
};
