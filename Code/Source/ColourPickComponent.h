/*
  ==============================================================================

    ColourPickComponent.h
    Created: 7 Mar 2025 3:05:24pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SignalManagerUI.h"

//==============================================================================
/*
*/
class ColourPickComponent  : public juce::Component
{
public:
    ColourPickComponent(juce::ValueTree& channelSettings);
    ~ColourPickComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    void doUpdateColour();
    void cancelUpdate();

    juce::FlexBox flexBox;
    juce::ColourSelector colourSelector;
    juce::FlexBox buttonFlexBox;
    juce::TextButton updateColour;
    juce::TextButton cancel;
    //colourSelector->setName("Background");
    //colourSelector->setCurrentColour(findColour(juce::DocumentWindow::backgroundColourId));
    juce::ValueTree channelSettings;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColourPickComponent)
};
