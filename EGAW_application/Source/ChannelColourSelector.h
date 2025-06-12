/*
  ==============================================================================

    ChannelColourSelector.h
    Created: 7 Mar 2025 9:45:46am
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ColourPickComponent.h"

//==============================================================================
/*
*/
class ChannelColourSelector  : public juce::ImageButton
{
public:
    ChannelColourSelector(juce::ValueTree&);
    ~ChannelColourSelector() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void mouseEnter(const juce::MouseEvent&) override;
    void mouseExit(const juce::MouseEvent&) override;

private:

    void openColorSelector();

    juce::ValueTree channelSettings;
    std::unique_ptr<juce::DialogWindow> dialogWindow;
    juce::Image buttonImage;
    float opacity = 0.8f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChannelColourSelector)
};
