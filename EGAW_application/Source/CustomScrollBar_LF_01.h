/*
  ==============================================================================

    CustomScrollBar_LF_01.h
    Created: 11 May 2025 1:32:47pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ColourConstants.h"

class CustomScrollBar_LF : public juce::LookAndFeel_V4
{
public:
    CustomScrollBar_LF()
    {
        // Set default colors (can be overridden with setColour)
        setColour(juce::ScrollBar::thumbColourId, colourConstants::dark_slate_blue_pressed); // Dark green
        //setColour(juce::ScrollBar::trackColourId, juce::Colours::transparentBlack);
        //setColour(juce::ScrollBar::backgroundColourId, juce::Colours::transparentBlack);
    }

    void drawScrollbar(juce::Graphics& g, juce::ScrollBar& scrollbar,
        int x, int y, int width, int height,
        bool isScrollbarVertical, int thumbStartPosition,
        int thumbSize, bool isMouseOver, bool isMouseDown) override
    {
        juce::ignoreUnused(x, y); // Parameters we won't use

        
        auto track = scrollbar.getLocalBounds().toFloat();

        // Draw track background (optional)
        /*
        if (!lengthReached) g.setColour(colourConstants::plain_green);
        else g.setColour(colourConstants::plain_green.darker(0.5f));
        */
        g.setColour(juce::Colours::lightgrey.darker(0.1f));
        g.fillRect(track);

        g.fillAll(juce::Colours::transparentBlack);

        // Thumb rectangle
        juce::Rectangle<float> thumb;

        if (isScrollbarVertical)
        {
            thumb = { 0.0f, (float)thumbStartPosition,
                     (float)width, (float)thumbSize };
        }
        else
        {
            thumb = { (float)thumbStartPosition, 0.0f,
                     (float)thumbSize, (float)height };
        }

        // Custom colors
        auto baseColour = scrollbar.findColour(juce::ScrollBar::thumbColourId);
        auto thumbColour = baseColour;

        if (isMouseOver && isMouseDown) thumbColour = thumbColour.darker(0.2f);
        else if (isMouseOver) thumbColour = thumbColour.brighter(0.2f);

        // Draw thumb with white border
        g.setColour(thumbColour);
        g.fillRect(thumb);

        g.setColour(juce::Colours::lightgrey.darker(0.1f)); // White border
        g.drawRect(thumb, 2.0f); // 1 pixel border
    }

    // Make scrollbar wider
    int getDefaultScrollbarWidth() override
    {
        return 16; // Wider than default (default is usually 12)
    }

	void setLengthReached(bool reached)
	{
		lengthReached = reached;
	}

    // Optional: Remove the button areas at the ends
    // bool shouldDrawScrollbarButton(juce::Button&) override { return false; }

private:

    bool lengthReached = false;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomScrollBar_LF)
};