/*
  ==============================================================================

    VolumeSlider.h
    Created: 26 May 2025 3:43:57pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "VolumeSlider_LF.h"

class VolumeSlider : public juce::Slider
{
public:
	VolumeSlider();
	~VolumeSlider() override;
	//void paint(juce::Graphics& g) override;
	//void resized() override;

private:

	void setDataMode(bool isVisible);

	void mouseDrag(const juce::MouseEvent& event) override;
	void mouseDoubleClick(const juce::MouseEvent& event) override;
	void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel) override;
	void mouseDown(const juce::MouseEvent& event) override;
	void mouseUp(const juce::MouseEvent& event) override;

	VolumeSlider_LF lookAndFeel;

	juce::Label label;
	bool isTextActive;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VolumeSlider)
};