/*
  ==============================================================================

    PluginContainerMoveContainer.cpp
    Created: 23 Apr 2025 7:22:28pm
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PluginContainerMoveContainer.h"

//==============================================================================
PluginContainerMoveContainer::PluginContainerMoveContainer(juce::ValueTree& pluginState, juce::String pluginUuid)
    : flexContainer(juce::FlexBox::Direction::column, juce::FlexBox::Wrap::noWrap
        , juce::FlexBox::AlignContent::flexStart, juce::FlexBox::AlignItems::flexStart
        , juce::FlexBox::JustifyContent::flexStart)
    , pluginState(pluginState)
    , moveUpButton(pluginState, pluginUuid)
    , moveDownButton(pluginState, pluginUuid)
{
    moveUpButton.setButtonText("Move up");
    addAndMakeVisible(moveUpButton);

	moveDownButton.setButtonText("Move down");
	addAndMakeVisible(moveDownButton);
}

PluginContainerMoveContainer::~PluginContainerMoveContainer()
{
}

void PluginContainerMoveContainer::paint (juce::Graphics& g)
{
    
}

void PluginContainerMoveContainer::resized()
{
    auto area = getLocalBounds();

    flexContainer.items.clear();

    flexContainer.items.add(juce::FlexItem(moveUpButton).withHeight((float)area.getHeight()).withWidth(area.getWidth()));
    flexContainer.items.add(juce::FlexItem(moveDownButton).withHeight((float)area.getHeight()).withWidth(area.getWidth()));

    flexContainer.performLayout(area);

}
