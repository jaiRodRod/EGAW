/*
  ==============================================================================

    DampingComponent.h
    Created: 27 May 2025 7:23:37pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "DampingSlider.h"

class DampingComponent : public juce::Component
{
public:
	DampingComponent();
	~DampingComponent() override;
	void paint(juce::Graphics&) override;
	void resized() override;

	DampingSlider& getDampingSlider() { return dampingSlider; }

private:

	juce::FlexBox flexBox;

	DampingSlider dampingSlider;
	juce::Label dampingLabel;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DampingComponent);
};