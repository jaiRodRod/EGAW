/*
  ==============================================================================

    PluginSelectorWindow.cpp
    Created: 26 Apr 2025 4:15:47pm
    Author:  j

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PluginSelectorWindow.h"

//==============================================================================

PluginSelectorWindow::PluginSelectorWindow(juce::KnownPluginList& list)
    : DialogWindow("Select Plugin", juce::Colours::lightgrey, true)
{
    selector = std::make_unique<PluginSelectorComponent>(list);

    selector->onPluginSelected = [this](const juce::PluginDescription* desc) {
        if (onPluginSelected)
            onPluginSelected(desc);    
    };

    setContentOwned(selector.release(), true);

    setSize(400, 500);
    setResizable(true, false);
    setUsingNativeTitleBar(true);
}
