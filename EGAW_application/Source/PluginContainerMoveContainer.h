/*
  ==============================================================================

    PluginContainerMoveContainer.h
    Created: 23 Apr 2025 7:22:28pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "PluginContainerMoveUpButton.h"
#include "PluginContainerMoveDownButton.h"

//==============================================================================
/*
*/
class PluginContainerMoveContainer  : public juce::Component
{
public:
    PluginContainerMoveContainer(juce::ValueTree& pluginState, juce::String pluginUuid);
    ~PluginContainerMoveContainer() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::ValueTree pluginState;

    //juce::Grid channelVerticalGrid;
    juce::FlexBox flexContainer;
    PluginContainerMoveUpButton moveUpButton;
	PluginContainerMoveDownButton moveDownButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginContainerMoveContainer)
};
