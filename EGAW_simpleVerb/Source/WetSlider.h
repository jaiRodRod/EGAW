/*
  ==============================================================================

    WetSlider.h
    Created: 27 May 2025 5:29:16pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "VerticalFader_LF.h"

class WetSlider : public juce::Slider
{
public:
	WetSlider();
	~WetSlider() override;
	//void paint(juce::Graphics& g) override;
	//void resized() override;

private:

	void setDataMode(bool isVisible);

	void mouseDrag(const juce::MouseEvent& event) override;
	void mouseDoubleClick(const juce::MouseEvent& event) override;
	void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel) override;
	void mouseDown(const juce::MouseEvent& event) override;
	void mouseUp(const juce::MouseEvent& event) override;

	VerticalFader_LF verticalFaderLookAndFeel;

	juce::Label wetLabel;
	juce::Label wetLevelLabel;
	bool showingLevel;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WetSlider)
};