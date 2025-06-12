/*
  ==============================================================================

    WidthComponent.cpp
    Created: 27 May 2025 7:23:54pm
    Author:  jaezro

  ==============================================================================
*/

#include "WidthComponent.h"

WidthComponent::WidthComponent() : flexBox(juce::FlexBox::Direction::column, juce::FlexBox::Wrap::noWrap, juce::FlexBox::AlignContent::center,
	juce::FlexBox::AlignItems::center, juce::FlexBox::JustifyContent::center), widthLabel("WidthLabel", "Width"), widthSlider()
{

	widthLabel.setJustificationType(juce::Justification::centred);

	addAndMakeVisible(widthLabel);
	addAndMakeVisible(widthSlider);
}

WidthComponent::~WidthComponent()
{
}

void WidthComponent::paint(juce::Graphics&)
{
}

void WidthComponent::resized()
{
	auto area = getLocalBounds().reduced(10);
	flexBox.items.clear();

	flexBox.items.add(juce::FlexItem(widthLabel).withMinHeight(area.getHeight() * 0.15f).withMinWidth(area.getWidth()));
	flexBox.items.add(juce::FlexItem(widthSlider).withMinHeight(area.getHeight() * 0.65f).withMinWidth(area.getWidth()));
	flexBox.items.add(juce::FlexItem().withMinHeight(area.getHeight() * 0.2f).withMinWidth(area.getWidth()));

	flexBox.performLayout(area);
}
