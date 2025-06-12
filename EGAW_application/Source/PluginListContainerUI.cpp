/*
  ==============================================================================

    PluginListContainerUI.cpp
    Created: 23 Apr 2025 3:39:32pm
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PluginListContainerUI.h"

//==============================================================================
PluginListContainerUI::PluginListContainerUI(juce::ValueTree& projectData, juce::ValueTree& pluginState, juce::String channelUuid) 
	: projectData(projectData)
    , pluginState(pluginState)
    , flexContainer(juce::FlexBox::Direction::column, juce::FlexBox::Wrap::noWrap
        , juce::FlexBox::AlignContent::flexStart, juce::FlexBox::AlignItems::flexStart
        , juce::FlexBox::JustifyContent::flexStart)
	, channelUuid(channelUuid)
{
    for (int i = 0; i < pluginState.getChildWithName("pluginOrder").getNumChildren(); ++i)
    {
		auto pluginUuid = pluginState.getChildWithName("pluginOrder").getChild(i).getType().toString();
		auto pluginSettings = pluginState.getChildWithName(pluginUuid);

		//auto colourID = projectData.getChildWithName(channelUuid).getProperty("Colour").toString();
		PluginContainerUI* pluginContainer = new PluginContainerUI(pluginState, pluginSettings, pluginUuid, projectData, channelUuid);
		pluginContainers.add(pluginContainer);
		addAndMakeVisible(pluginContainer);
    }

    this->pluginState.addListener(this);
}

PluginListContainerUI::~PluginListContainerUI()
{
	pluginState.removeListener(this);
}

void PluginListContainerUI::paint (juce::Graphics& g)
{
	auto colourID = projectData.getChildWithName(channelUuid).getProperty("Colour").toString();
	g.fillAll (juce::Colour::fromString(colourID));
}

void PluginListContainerUI::resized()
{
    auto area = getLocalBounds();

    flexContainer.items.clear();

    int numPosition = 1;
    for (juce::ValueTree pluginIdUnderOrder : pluginState.getChildWithName("pluginOrder"))
    {
        juce::String pluginUuid = pluginIdUnderOrder.getType().toString();
        juce::ValueTree plugin = pluginState.getChildWithName(pluginUuid);
        plugin.setProperty("pluginPosition", numPosition, nullptr);

        auto* pluginContainer = getPluginContainerUI(pluginUuid);
        pluginContainer->setSize(area.getWidth(), area.getHeight() / 5);
        flexContainer.items.add(juce::FlexItem(*pluginContainer).withMinHeight((float)area.getHeight()/5).withMinWidth((float)area.getWidth()));

        ++numPosition;
    }

    flexContainer.performLayout(area);
}

void PluginListContainerUI::valueTreeChildOrderChanged(juce::ValueTree& parentTreeWhoseChildrenHaveBeenMoved, int oldIndex, int newIndex)
{
    if (parentTreeWhoseChildrenHaveBeenMoved == pluginState.getChildWithName("pluginOrder"))
    {
        resized();
    }
}

PluginContainerUI* PluginListContainerUI::getPluginContainerUI(juce::String pluginUuid)
{
    for (int i = 0; i < pluginContainers.size(); ++i)
    {
        auto* pluginContainerUI = pluginContainers.getUnchecked(i);
        if (pluginContainerUI->getPluginUuid() == pluginUuid)
        {
            return pluginContainerUI;
        }
    }
    return nullptr;
}
