/*
  ==============================================================================

    PluginList.h
    Created: 20 Apr 2025 3:07:33pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "Plugin.h"

class PluginList
{
public:

	PluginList(GlobalPlayhead&, juce::ValueTree&, juce::String);
	PluginList(GlobalPlayhead&, juce::ValueTree&, juce::String, juce::ValueTree);
	~PluginList();

	void prepareToPlay(int samplesPerBlock, double sampleRate);
	void releaseResources();
	void processAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);

private:

	Plugin* getPlugin(juce::String pluginUuid);


	juce::ValueTree pluginState;
	juce::String channelUuid;
	juce::OwnedArray<Plugin> plugins;

};