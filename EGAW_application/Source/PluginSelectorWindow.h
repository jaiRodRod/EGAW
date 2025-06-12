/*
  ==============================================================================

    PluginSelectorWindow.h
    Created: 26 Apr 2025 4:15:47pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "PluginSelectorComponent.h"

//==============================================================================
/*
*/
class PluginSelectorWindow  : public juce::DialogWindow
{
public:
    PluginSelectorWindow(juce::KnownPluginList& list);
	~PluginSelectorWindow() override = default;

	void closeButtonPressed() override
	{
		setVisible(false);
	}

    std::function<void(const juce::PluginDescription*)> onPluginSelected;

private:

    std::unique_ptr<PluginSelectorComponent> selector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginSelectorWindow)
};
