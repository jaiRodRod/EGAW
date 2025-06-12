/*
  ==============================================================================

    PluginUI.h
    Created: 23 Apr 2025 3:41:01pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginActionStateManager.h"

//==============================================================================
/*
*/
class PluginUI : public juce::DocumentWindow
{
public:
    PluginUI(juce::AudioProcessorEditor* pluginEditor, juce::String pluginUuid)
        : DocumentWindow(pluginEditor->getName(),
            juce::Colours::lightgrey,
            DocumentWindow::closeButton)
		, pluginUuid(pluginUuid)
    {
        setUsingNativeTitleBar(true);
        setContentOwned(pluginEditor, true);
        setResizable(true, false); // Allow resizing but not aspect ratio
        centreWithSize(getWidth(), getHeight());

        /*
		juce::MessageManager::getInstance()->callAsync([this]()
			{
                if (auto* window = this)
                {
                    window->setVisible(true);
                }
			});
        */

        juce::MessageManager::callAsync([safeThis = juce::Component::SafePointer(this)] {
            if (safeThis != nullptr)
                safeThis->setVisible(true);
        });
    }

    ~PluginUI() override
    {
        // Remove the editor before destruction
        clearContentComponent();
    }

    void closeButtonPressed() override
    {
		PluginActionStateManager::getInstance().setPluginUuid(pluginUuid);
		PluginActionStateManager::getInstance().setSignal(PluginActionStateManager::PluginSignal::CLOSE_PLUGIN);
    }

private:

	juce::String pluginUuid;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginUI)
};
