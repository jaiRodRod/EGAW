/*
  ==============================================================================

    PluginContainerMoveUpButton.h
    Created: 23 Apr 2025 7:22:44pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PluginContainerMoveUpButton  : public juce::TextButton
{
public:

    PluginContainerMoveUpButton(juce::ValueTree& pluginState, juce::String pluginUuid);
    ~PluginContainerMoveUpButton() override;

	void paint (juce::Graphics&) override;
	void resized() override;

private:

    void moveUpAction();

    juce::String pluginUuid;
    juce::ValueTree pluginState;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginContainerMoveUpButton)
};
