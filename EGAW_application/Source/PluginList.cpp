/*
  ==============================================================================

    PluginList.cpp
    Created: 20 Apr 2025 3:07:33pm
    Author:  jaezro

  ==============================================================================
*/

#include "PluginList.h"

PluginList::PluginList(GlobalPlayhead& globalPlayhead, juce::ValueTree& pluginState, juce::String channelUuid) : pluginState(pluginState), channelUuid(channelUuid)
{
    juce::ValueTree pluginOrder("pluginOrder");
    pluginState.addChild(pluginOrder, -1, nullptr);

    for (int i = 0; i < 5; ++i)
    {
		plugins.add(new Plugin(globalPlayhead, pluginState));
    }
}

PluginList::PluginList(GlobalPlayhead& globalPlayhead, juce::ValueTree& pluginState, juce::String channelUuid, juce::ValueTree pluginStateRestorerValueTree) : pluginState(pluginState), channelUuid(channelUuid)
{
    juce::ValueTree pluginOrder("pluginOrder");
    pluginState.addChild(pluginOrder, -1, nullptr);

    for (int i = 0; i < 5; ++i)
    {
		auto pluginUuid = pluginStateRestorerValueTree.getChildWithName("pluginOrder").getChild(i).getType().toString();
        plugins.add(new Plugin(globalPlayhead, pluginState, pluginStateRestorerValueTree, pluginUuid));
    }
}

PluginList::~PluginList()
{

}

void PluginList::prepareToPlay(int samplesPerBlock, double sampleRate)
{
}

void PluginList::releaseResources()
{
}

void PluginList::processAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    for (juce::ValueTree pluginIdUnderOrder : pluginState.getChildWithName("pluginOrder"))
    {
        juce::String pluginUuid = pluginIdUnderOrder.getType().toString();
        juce::ValueTree plugin = pluginState.getChildWithName(pluginUuid);
        auto* pluginInstance = getPlugin(pluginUuid);

        if ((bool)plugin.getProperty("isBypassed") == false)
        {
            // Process
            pluginInstance->processAudioBlock(bufferToFill);
        }
    }
}

Plugin* PluginList::getPlugin(juce::String pluginUuid)
{
    for (int i = 0; i < plugins.size(); ++i)
    {
        auto* plugin = plugins.getUnchecked(i);
        if (plugin->getPluginUuid() == pluginUuid)
        {
            return plugin;
        }
    }
    return nullptr;
}
