/*
  ==============================================================================

    PluginListContainerUI.h
    Created: 23 Apr 2025 3:39:32pm
    Author:  j

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "PluginContainerUI.h"

//==============================================================================
/*
*/
class PluginListContainerUI : public juce::Component, public juce::ValueTree::Listener
{
public:
    PluginListContainerUI(juce::ValueTree&, juce::ValueTree&, juce::String);
    ~PluginListContainerUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void valueTreeChildOrderChanged(juce::ValueTree& parentTreeWhoseChildrenHaveBeenMoved, int oldIndex, int newIndex) override;

private:

    PluginContainerUI* getPluginContainerUI(juce::String pluginUuid);

    juce::ValueTree projectData;
	juce::ValueTree pluginState;
    juce::String channelUuid;

	juce::FlexBox flexContainer;
	juce::OwnedArray<PluginContainerUI> pluginContainers;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginListContainerUI)
};
