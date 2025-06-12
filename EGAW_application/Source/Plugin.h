/*
  ==============================================================================

    Plugin.h
    Created: 20 Apr 2025 3:07:52pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "PluginActionStateManager.h"
#include "GlobalPlayhead.h"
#include "PluginUI.h"

class Plugin : public juce::MessageListener, public juce::Timer
{
public:
	Plugin(GlobalPlayhead&, juce::ValueTree&);
	Plugin(GlobalPlayhead&, juce::ValueTree&, juce::ValueTree, juce::String);
	~Plugin();

	void handleMessage(const juce::Message& message) override;

	void prepareToPlay(int samplesPerBlock, double sampleRate);
	void releaseResources();
	void processAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);

	juce::ValueTree& getPluginSettings() { return pluginSettings; }
	juce::String getPluginUuid() const { return pluginUuid.toString(); }

	void loadPlugin(const juce::PluginDescription& description);
	void reloadPlugin(const juce::PluginDescription& description, juce::ValueTree pluginStateRestorerValueTree);

private:

	void doAddPlugin();
	void doOpenPlugin();
	void doClosePlugin();
	void doRemovePlugin();

	void doSavePlugin();
	void timerCallback() override;

	juce::ValueTree pluginState;
	juce::Uuid pluginUuid;

	juce::ValueTree pluginSettings;

	bool isUnused;

	GlobalPlayhead& globalPlayhead;
	std::unique_ptr<juce::AudioPluginInstance> pluginInstance;
	juce::AudioPluginFormatManager& pluginFormatManager;

	std::unique_ptr<PluginUI> pluginUI;
};