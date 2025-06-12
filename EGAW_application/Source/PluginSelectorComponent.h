/*
  ==============================================================================

    PluginSelectorComponent.h
    Created: 26 Apr 2025 4:15:28pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PluginSelectorComponent  : public juce::Component, private juce::ListBoxModel
{
public:
    
    PluginSelectorComponent(juce::KnownPluginList& list);

    std::function<void(const juce::PluginDescription*)> onPluginSelected;

    void resized() override;


private:

    int getNumRows() override { return pluginList.getNumTypes(); }

    void paintListBoxItem(int row, juce::Graphics& g, int width, int height, bool rowIsSelected) override;

    void listBoxItemDoubleClicked(int row, const juce::MouseEvent&) override;

    juce::KnownPluginList& pluginList;
    juce::ListBox listBox{ "PluginList" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginSelectorComponent)
};
