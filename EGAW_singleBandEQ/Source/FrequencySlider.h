/*
  ==============================================================================

    FrequencySlider.h
    Created: 26 May 2025 3:44:13pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "FrequencySlider_LF.h"

class FrequencySlider : public juce::Slider
{
public:
	FrequencySlider();
	~FrequencySlider() override;
	//void paint(juce::Graphics& g) override;
	//void resized() override;

private:

	void setDataMode(bool isVisible);

	void mouseDrag(const juce::MouseEvent& event) override;
	void mouseDoubleClick(const juce::MouseEvent& event) override;
	void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel) override;
	void mouseDown(const juce::MouseEvent& event) override;
	void mouseUp(const juce::MouseEvent& event) override;

	FrequencySlider_LF lookAndFeel;

	juce::Label label;
	bool isTextActive;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FrequencySlider);
};