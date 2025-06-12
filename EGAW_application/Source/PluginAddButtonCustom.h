/*
  ==============================================================================

    PluginAddButtonCustom.h
    Created: 22 May 2025 4:16:55pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class PluginAddButtonCustom : public juce::TextButton
{
public:
	PluginAddButtonCustom(juce::ValueTree& projectData, juce::String channelUuid);
	~PluginAddButtonCustom() override;
	void paint (juce::Graphics& g) override;
	void resized() override;

	void mouseEnter (const juce::MouseEvent& event) override
	{
		mouseOver = true;
		repaint();
	}
	void mouseExit (const juce::MouseEvent& event) override
	{
		mouseOver = false;
		repaint();
	}

private:
	juce::ValueTree projectData;
	juce::String channelUuid;
	bool mouseOver = false;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginAddButtonCustom)
};