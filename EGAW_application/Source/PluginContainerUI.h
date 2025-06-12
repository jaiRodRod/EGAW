/*
  ==============================================================================

    PluginContainerUI.h
    Created: 23 Apr 2025 3:40:47pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "PluginActionStateManager.h"

#include "PluginSelectorWindow.h"

#include "PluginAddButtonCustom.h"

#include "PluginContainerTextName.h"
#include "PluginContainerMoveContainer.h"
#include "PluginComponentBypassToggleButton.h"
#include "PluginContainerDeleteButton.h"
//==============================================================================
/*
*/
class PluginContainerUI  : public juce::Component, public juce::ValueTree::Listener
{
public:
    PluginContainerUI(juce::ValueTree& pluginState, juce::ValueTree& pluginSettings, juce::String pluginUuid, juce::ValueTree& projectData, juce::String channelUuid);
    ~PluginContainerUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override;

	juce::String getPluginUuid() const { return pluginUuid; }

private:

	void addPlugin();
	void showPluginSelector();

	juce::ValueTree pluginState;
	juce::ValueTree pluginSettings;
	juce::String pluginUuid;
	juce::ValueTree projectData;
	juce::String channelUuid;

	//Plugin show
	juce::KnownPluginList& pluginList;
	juce::AudioPluginFormatManager& formatManager;

	juce::FlexBox flexContainer;

    PluginAddButtonCustom addPluginButton;

	PluginContainerTextName pluginContainerTextName;
	PluginContainerMoveContainer pluginContainerMoveContainer;
	PluginComponentBypassToggleButton pluginContainerBypassButton;
	PluginContainerDeleteButton pluginContainerDeleteButton; 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginContainerUI)
};
