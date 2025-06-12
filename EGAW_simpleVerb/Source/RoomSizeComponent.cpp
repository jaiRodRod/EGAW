/*
  ==============================================================================

    RoomSizeComponent.cpp
    Created: 27 May 2025 6:49:57pm
    Author:  jaezro

  ==============================================================================
*/

#include "RoomSizeComponent.h"

RoomSizeComponent::RoomSizeComponent() : flexBox(juce::FlexBox::Direction::column, juce::FlexBox::Wrap::noWrap, juce::FlexBox::AlignContent::center,
	juce::FlexBox::AlignItems::center, juce::FlexBox::JustifyContent::center), roomSizeLabel("RoomSizeLabel", "Room Size"), roomSizeSlider()
{

	roomSizeLabel.setJustificationType(juce::Justification::centred);

	addAndMakeVisible(roomSizeLabel);
	addAndMakeVisible(roomSizeSlider);
}

RoomSizeComponent::~RoomSizeComponent()
{
}

void RoomSizeComponent::paint(juce::Graphics&)
{
}

void RoomSizeComponent::resized()
{
	auto area = getLocalBounds().reduced(10);
	flexBox.items.clear();

	flexBox.items.add(juce::FlexItem(roomSizeLabel).withMinHeight(area.getHeight() * 0.15f).withMinWidth(area.getWidth()));
	flexBox.items.add(juce::FlexItem(roomSizeSlider).withMinHeight(area.getHeight() * 0.65f).withMinWidth(area.getWidth()));
	flexBox.items.add(juce::FlexItem().withMinHeight(area.getHeight() * 0.2f).withMinWidth(area.getWidth()));

	flexBox.performLayout(area);
}
