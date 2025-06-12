/*
  ==============================================================================

    PluginContainerDeleteButton.cpp
    Created: 23 Apr 2025 7:23:29pm
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PluginContainerDeleteButton.h"

//==============================================================================
PluginContainerDeleteButton::PluginContainerDeleteButton(juce::ValueTree& pluginSettings, juce::String pluginUuid) : pluginSettings(pluginSettings), pluginUuid(pluginUuid)
{
	buttonImage = juce::ImageCache::getFromMemory(BinaryData::delete_icon_png, BinaryData::delete_icon_pngSize);

	setMouseCursor(juce::MouseCursor::PointingHandCursor);
	setTooltip("Click to delete plugin");
    setButtonText("Delete plugin");
    onClick = [this] {showAcceptPopUp();};
}

PluginContainerDeleteButton::~PluginContainerDeleteButton()
{
}

void PluginContainerDeleteButton::paint(juce::Graphics& g)
{
	if (buttonImage.isValid())
	{
		// Draw image with current opacity
		g.setOpacity(0.8);

		// Scale image to fit while maintaining aspect ratio
		auto bounds = getLocalBounds().toFloat().reduced(5); // Small margin
		bounds.setX(bounds.getX() - 4);
		juce::RectanglePlacement placement(juce::RectanglePlacement::centred |
			juce::RectanglePlacement::onlyReduceInSize);

		g.drawImage(buttonImage,
			bounds,
			placement,
			false); // Don't fill alpha channel
	}
}

void PluginContainerDeleteButton::resized()
{
	repaint();
}

void PluginContainerDeleteButton::showAcceptPopUp()
{
	bool result = juce::AlertWindow::showOkCancelBox(
		juce::AlertWindow::QuestionIcon,
		"Remove plugin",
		"¿Are you sure you want to remove the plugin?",
		"Yes",
		"No",
		nullptr,
		juce::ModalCallbackFunction::create([this](int result)
			{
				if (result == 1)
				{
					// Handle deletion here
					deletePlugin();
				}
			})
	);
}

void PluginContainerDeleteButton::deletePlugin()
{
	// TODO: Implement plugin deletion logic

	PluginActionStateManager::getInstance().setPluginUuid(pluginUuid);
	PluginActionStateManager::getInstance().setSignal(PluginActionStateManager::PluginSignal::REMOVE_PLUGIN);
}
