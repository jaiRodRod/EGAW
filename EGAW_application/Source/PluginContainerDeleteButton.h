/*
  ==============================================================================

    PluginContainerDeleteButton.h
    Created: 23 Apr 2025 7:23:29pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginActionStateManager.h"

//==============================================================================
/*
*/
class PluginContainerDeleteButton : public juce::TextButton
{
public:
    PluginContainerDeleteButton(juce::ValueTree& pluginSettings, juce::String pluginUuid);
    ~PluginContainerDeleteButton() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    void showAcceptPopUp();
    void deletePlugin();

	juce::ValueTree pluginSettings;
	juce::String pluginUuid;
    juce::Image buttonImage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginContainerDeleteButton)
};
