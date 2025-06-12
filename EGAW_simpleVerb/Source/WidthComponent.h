/*
  ==============================================================================

    WidthComponent.h
    Created: 27 May 2025 7:23:54pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "WidthSlider.h"

class WidthComponent : public juce::Component
{
public:
	WidthComponent();
	~WidthComponent() override;
	void paint(juce::Graphics&) override;
	void resized() override;

	WidthSlider& getWidthSlider() { return widthSlider; }

private:

	juce::FlexBox flexBox;

	WidthSlider widthSlider;
	juce::Label widthLabel;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WidthComponent);
};