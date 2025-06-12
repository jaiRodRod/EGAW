/*
  ==============================================================================

    RoomSizeComponent.h
    Created: 27 May 2025 6:49:57pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "RoomSizeSlider.h"

class RoomSizeComponent : public juce::Component
{
public:
	RoomSizeComponent();
	~RoomSizeComponent() override;
	void paint(juce::Graphics&) override;
	void resized() override;

	RoomSizeSlider& getRoomSizeSlider() { return roomSizeSlider; }

private:

	juce::FlexBox flexBox;

	RoomSizeSlider roomSizeSlider;
	juce::Label roomSizeLabel;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RoomSizeComponent);
};