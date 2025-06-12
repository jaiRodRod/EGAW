/*
  ==============================================================================

    CustomToggleButton_LF_01.h
    Created: 8 May 2025 7:07:01pm
    Author:  j

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ColourConstants.h"

class CustomToggleButton_LF : public juce::LookAndFeel_V4
{
public:
    void drawToggleButton(juce::Graphics& g,
        juce::ToggleButton& button,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) override
    {
        // Background color based on toggle state
        auto bgColor = colourConstants::dark_green;
        if (button.getToggleState() == false && (button.isMouseOver()))
        {
            bgColor = bgColor.brighter(0.1f); // 10% brighter on hover
        }
        if (button.getToggleState() == true)
        {
            bgColor = colourConstants::darker_slate_blue_pressed;
        }

        // Draw background rectangle
        g.setColour(bgColor);
        g.fillRoundedRectangle(button.getLocalBounds().toFloat(), 0.0f);

        // Draw border
        /*
        g.setColour(juce::Colours::white.withAlpha(0.5f));
        g.drawRoundedRectangle(button.getLocalBounds().toFloat(), 0.0f, 1.0f);
        */

        // Draw text (customized)
		auto textColour = button.getToggleState() ? juce::Colours::white : juce::Colours::white;
        if (button.isMouseOver())
        {
            textColour = textColour.brighter(0.2f); // Text slightly brighter too
        }

        g.setColour(textColour);
        auto fontSize = juce::jmin(button.getHeight() * 0.8f, button.getWidth() * 0.4f); // Adjust factors as needed
        g.setFont(juce::Font(fontSize, juce::Font::bold)); // Custom font
        g.drawText(button.getButtonText(),
            button.getLocalBounds(),
            juce::Justification::centred,
            true);
    }
};