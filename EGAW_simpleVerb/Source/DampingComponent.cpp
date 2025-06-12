/*
  ==============================================================================

    DampingComponent.cpp
    Created: 27 May 2025 7:23:37pm
    Author:  jaezro

  ==============================================================================
*/

#include "DampingComponent.h"

DampingComponent::DampingComponent() : flexBox(juce::FlexBox::Direction::column, juce::FlexBox::Wrap::noWrap, juce::FlexBox::AlignContent::center,
	juce::FlexBox::AlignItems::center, juce::FlexBox::JustifyContent::center), dampingLabel("DampingLabel", "Damping"), dampingSlider()
{

	dampingLabel.setJustificationType(juce::Justification::centred);

	addAndMakeVisible(dampingLabel);
	addAndMakeVisible(dampingSlider);
}

DampingComponent::~DampingComponent()
{
}

void DampingComponent::paint(juce::Graphics&)
{
}

void DampingComponent::resized()
{
	auto area = getLocalBounds().reduced(10);
	flexBox.items.clear();

	flexBox.items.add(juce::FlexItem(dampingLabel).withMinHeight(area.getHeight() * 0.15f).withMinWidth(area.getWidth()));
	flexBox.items.add(juce::FlexItem(dampingSlider).withMinHeight(area.getHeight() * 0.65f).withMinWidth(area.getWidth()));
	flexBox.items.add(juce::FlexItem().withMinHeight(area.getHeight() * 0.2f).withMinWidth(area.getWidth()));

	flexBox.performLayout(area);
}
