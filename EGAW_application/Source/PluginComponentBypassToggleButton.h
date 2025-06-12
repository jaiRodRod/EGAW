/*
  ==============================================================================

    PluginComponentBypassToggleButton.h
    Created: 23 Apr 2025 7:23:13pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SignalManagerUI.h"

#include "BinaryData.h"

#include "ColourConstants.h"

//==============================================================================
/*
*/
class PluginComponentBypassToggleButton  : public juce::ToggleButton, public juce::MessageListener
{
public:
    PluginComponentBypassToggleButton(juce::ValueTree& pluginSettings);
    ~PluginComponentBypassToggleButton() override;

    void paint (juce::Graphics& g) override;
    //void resized() override;

    void handleMessage(const juce::Message& message) override;

private:

    void setBypassed(bool);

    juce::ValueTree pluginSettings;
    std::unique_ptr<juce::Drawable> svgDrawable;
    bool shouldShowTooltip = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginComponentBypassToggleButton)
};
