/*
  ==============================================================================

    PluginSelectorComponent.cpp
    Created: 26 Apr 2025 4:15:28pm
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PluginSelectorComponent.h"

//==============================================================================

PluginSelectorComponent::PluginSelectorComponent(juce::KnownPluginList& list)
    : pluginList(list)
{
    addAndMakeVisible(listBox);
    listBox.setModel(this);
    listBox.setRowHeight(24);
}

void PluginSelectorComponent::resized()
{
    listBox.setBounds(getLocalBounds());
}

void PluginSelectorComponent::paintListBoxItem(int row, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
    if (row >= pluginList.getNumTypes())
        return;

    auto& desc = pluginList.getTypes()[row];

    if (rowIsSelected)
        g.fillAll(juce::Colours::lightblue);

    g.setColour(juce::Colours::white);
    g.setFont(height * 0.7f);

    g.drawText(desc.name, 5, 0, width - 5, height, juce::Justification::centredLeft);
}

void PluginSelectorComponent::listBoxItemDoubleClicked(int row, const juce::MouseEvent&)
{
    if (onPluginSelected && row < pluginList.getNumTypes())
        onPluginSelected(&pluginList.getTypes()[row]);
}