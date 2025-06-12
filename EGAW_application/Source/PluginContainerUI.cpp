/*
  ==============================================================================

    PluginContainerUI.cpp
    Created: 23 Apr 2025 3:40:47pm
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PluginContainerUI.h"

//==============================================================================
PluginContainerUI::PluginContainerUI(juce::ValueTree& pluginState, juce::ValueTree& pluginSettings, juce::String pluginUuid, juce::ValueTree& projectData, juce::String channelUuid)
	: pluginState(pluginState)
	, pluginSettings(pluginSettings)
	, pluginUuid(pluginUuid)
	, projectData(projectData)
	, channelUuid(channelUuid)
    , flexContainer(juce::FlexBox::Direction::row, juce::FlexBox::Wrap::noWrap
        , juce::FlexBox::AlignContent::flexStart, juce::FlexBox::AlignItems::flexStart
        , juce::FlexBox::JustifyContent::flexStart)
	, addPluginButton(projectData, channelUuid)
	, pluginList(*PluginActionStateManager::getInstance().getPluginList())
	, formatManager(*PluginActionStateManager::getInstance().getPluginFormatManager())
	, pluginContainerTextName(pluginSettings, pluginUuid, projectData)
	, pluginContainerMoveContainer(pluginState, pluginUuid)
	, pluginContainerBypassButton(pluginSettings)
	, pluginContainerDeleteButton(pluginSettings, pluginUuid)
{
    
	addPluginButton.setButtonText("Add Plugin");
	addPluginButton.onClick = [this] { addPlugin(); };
	addAndMakeVisible(addPluginButton);

	pluginContainerTextName.setButtonText(pluginSettings.getProperty("Name"));
	addAndMakeVisible(pluginContainerTextName);

	addAndMakeVisible(pluginContainerMoveContainer);

	pluginContainerBypassButton.setButtonText("Bypass");
	addAndMakeVisible(pluginContainerBypassButton);

	addAndMakeVisible(pluginContainerDeleteButton);

	this->pluginSettings.addListener(this);
}

PluginContainerUI::~PluginContainerUI()
{
	pluginSettings.removeListener(this);
}

void PluginContainerUI::paint (juce::Graphics& g)
{
    //g.fillAll (juce::Colours::grey);   // clear the background
}

void PluginContainerUI::resized()
{
	auto area = getLocalBounds();
	flexContainer.items.clear();
	if ((bool)pluginSettings.getProperty("isOccupied") == false)
	{
		addPluginButton.setVisible(true);
		pluginContainerTextName.setVisible(false);
		pluginContainerMoveContainer.setVisible(false);
		pluginContainerBypassButton.setVisible(false);
		pluginContainerDeleteButton.setVisible(false);
		flexContainer.items.add(juce::FlexItem(addPluginButton).withHeight((float)area.getHeight()).withWidth(area.getWidth()));
	}
	else
	{
		addPluginButton.setVisible(false);
		pluginContainerTextName.setVisible(true);
		pluginContainerMoveContainer.setVisible(true);
		pluginContainerBypassButton.setVisible(true);
		pluginContainerDeleteButton.setVisible(true);
		flexContainer.items.add(juce::FlexItem(pluginContainerTextName).withHeight((float)area.getHeight()).withWidth(area.getWidth()/2));
		flexContainer.items.add(juce::FlexItem(pluginContainerMoveContainer).withHeight((float)area.getHeight()).withWidth((area.getWidth() / 2) / 3));
		flexContainer.items.add(juce::FlexItem(pluginContainerBypassButton).withHeight((float)area.getHeight()).withWidth((area.getWidth() / 2) / 3));
		flexContainer.items.add(juce::FlexItem(pluginContainerDeleteButton).withHeight((float)area.getHeight()).withWidth((area.getWidth() / 2) / 3));
	}
	flexContainer.performLayout(area);
}

void PluginContainerUI::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
	if (treeWhosePropertyHasChanged == pluginSettings)
	{
		if (property.toString() == "isOccupied")
		{

			pluginContainerTextName.setButtonText(pluginSettings.getProperty("Name"));
			resized();

		}
	}
}

void PluginContainerUI::addPlugin()
{
	// Send signal to add plugin
	// Like routing we set channelUuid, pluginUuid and then send the signal so the plugin is added where we need it


	PluginActionStateManager::getInstance().setPluginUuid(pluginUuid);
	//PluginActionStateManager::getInstance().setSignal(PluginActionStateManager::PluginSignal::ADD_PLUGIN);
	showPluginSelector();
}

void PluginContainerUI::showPluginSelector()
{
	auto* selector = new PluginSelectorWindow(pluginList);
	selector->onPluginSelected = [this, selector](const juce::PluginDescription* desc) {
			if (desc != nullptr)
			{
				//Señal para hacer load en el plugin
				//plugin.loadPlugin(*desc);
				PluginActionStateManager::getInstance().setPluginDescription(*desc);
				PluginActionStateManager::getInstance().setSignal(PluginActionStateManager::PluginSignal::DO_ADD_PLUGIN);

				selector->exitModalState(0);
				selector->setVisible(false);
			}
		};

	selector->enterModalState(true,
		juce::ModalCallbackFunction::create(
			[selector](int) { delete selector; }
		),
		true
	);
}