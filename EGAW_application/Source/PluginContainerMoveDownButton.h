/*
  ==============================================================================

    PluginContainerMoveDownButton.h
    Created: 23 Apr 2025 7:22:58pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PluginContainerMoveDownButton  : public juce::TextButton
{
public:
    PluginContainerMoveDownButton(juce::ValueTree& pluginState, juce::String pluginUuid);
    ~PluginContainerMoveDownButton() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    void moveDownAction();

    juce::String pluginUuid;
    juce::ValueTree pluginState;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginContainerMoveDownButton)
};
