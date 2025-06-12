/*
  ==============================================================================

    PluginContainerTextName.h
    Created: 23 Apr 2025 7:21:22pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "SignalManagerUI.h"
#include "PluginActionStateManager.h"

//==============================================================================
/*
*/
class PluginContainerTextName  : public juce::TextButton,
    public juce::MessageListener
{
public:
    PluginContainerTextName(juce::ValueTree& pluginSettings, juce::String channelUuid, juce::ValueTree& projectData);
    ~PluginContainerTextName() override;

	void paint (juce::Graphics& g) override;

	void handleMessage(const juce::Message& msg) override;
	void mouseEnter (const juce::MouseEvent& event) override
	{
		mouseOver = true;
		repaint();
	}
	void mouseExit (const juce::MouseEvent& event) override
	{
		mouseOver = false;
		repaint();
	}

private:

    void openPlugin();

    bool mouseOver = false;

    juce::ValueTree pluginSettings;
    juce::String channelUuid;
	juce::ValueTree projectData;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginContainerTextName)
};
