/*
  ==============================================================================

    PluginContainerTextName.cpp
    Created: 23 Apr 2025 7:21:22pm
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PluginContainerTextName.h"

//==============================================================================
PluginContainerTextName::PluginContainerTextName(juce::ValueTree& pluginSettings, juce::String channelUuid, juce::ValueTree& projectData) 
    : pluginSettings(pluginSettings), channelUuid(channelUuid), projectData(projectData)
{
	SignalManagerUI::getInstance().addListener(this);

	setTooltip("Open Plugin");
	setMouseCursor(juce::MouseCursor::PointingHandCursor);
    setButtonText(pluginSettings.getProperty("Name"));
    onClick = [this] { openPlugin(); };

}

PluginContainerTextName::~PluginContainerTextName()
{
	SignalManagerUI::getInstance().removeListener(this);
}

void PluginContainerTextName::paint(juce::Graphics& g)
{
	//auto colourID = projectData.getChildWithName(channelUuid).getProperty("Colour").toString();
	//juce::Colour baseColour = juce::Colour::fromString(colourID);
	auto baseColour = juce::Colours::grey;
	auto slightlyDarkerColour = baseColour.darker(0.2f);
	auto darkerColour = baseColour.darker(0.5f);

	//g.fillAll(baseColour);
	if (mouseOver)
	{
		g.setColour(slightlyDarkerColour);
	}
	else
	{
		g.setColour(darkerColour);
	}
	g.fillRoundedRectangle(getLocalBounds().reduced(4).toFloat(), 4.0f);
	g.setColour(juce::Colours::white);
	g.drawText(getButtonText(), getLocalBounds(), juce::Justification::centred, true);
}

void PluginContainerTextName::handleMessage(const juce::Message& msg)
{
    if (const auto* signalMsg = dynamic_cast<const SignalMessage*>(&msg))
    {
        const auto signal = static_cast<SignalManagerUI::Signal>(signalMsg->getSignalType());

        switch (signal)
        {
        case SignalManagerUI::Signal::RESTORE_UI_PARAMETERS:
            setButtonText(pluginSettings.getProperty("Name"));
            break;
        default:
            break;
        }
    }
}

void PluginContainerTextName::openPlugin()
{
    // do open plugin

	PluginActionStateManager::getInstance().setChannelUuid(channelUuid);
	PluginActionStateManager::getInstance().setPluginUuid(pluginSettings.getType().toString());

	PluginActionStateManager::getInstance().setSignal(PluginActionStateManager::PluginSignal::OPEN_PLUGIN);
}
