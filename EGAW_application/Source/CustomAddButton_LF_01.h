/*
  ==============================================================================

    CustomAddButton_LF_01.h
    Created: 12 May 2025 5:18:46pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "ColourConstants.h"

class CustomAddButton_LF : public juce::LookAndFeel_V4
{
public:
    void drawButtonBackground(juce::Graphics& g,
        juce::Button& button,
        const juce::Colour& backgroundColour,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) override
    {
        auto bounds = button.getLocalBounds().toFloat();
        auto cutEdge = bounds.getWidth() / 28;
		auto borderThickness = 2.0f;

        juce::Path outerPath;

        // Create trapezoid shape with cut corners
        outerPath.startNewSubPath(0, 0);
        outerPath.lineTo(cutEdge, bounds.getBottom());
        outerPath.lineTo(bounds.getRight() - cutEdge, bounds.getBottom());
        outerPath.lineTo(bounds.getRight(), 0);
        outerPath.closeSubPath();

        auto outerColor = colourConstants::dark_green.darker(0.5f);
        g.setColour(outerColor);
        g.fillPath(outerPath);

        juce::Path innerPath;

        // Create trapezoid shape with cut corners
        innerPath.startNewSubPath(borderThickness, borderThickness);
        innerPath.lineTo(cutEdge + borderThickness, bounds.getBottom() - borderThickness);
        innerPath.lineTo(bounds.getRight() - cutEdge - borderThickness, bounds.getBottom() - borderThickness);
        innerPath.lineTo(bounds.getRight() - borderThickness, borderThickness);
        innerPath.closeSubPath();

        auto innerColor = colourConstants::dark_green;
        if (button.isMouseOver(true)) innerColor = innerColor.brighter(0.15f);
        g.setColour(innerColor);
        g.fillPath(innerPath);
    }

    void drawButtonText(juce::Graphics& g,
        juce::TextButton& button,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) override
    {
        auto bounds = button.getLocalBounds().toFloat();
        float fontSize = calculateFontSize(bounds);

        g.setFont(getFontForButton(button, fontSize));
        g.setColour(button.findColour(button.getToggleState() ?
            juce::TextButton::textColourOnId :
            juce::TextButton::textColourOffId));

        g.drawText(button.getButtonText(), bounds,
            juce::Justification::centred, false);
    }

private:

    float calculateFontSize(const juce::Rectangle<float>& bounds)
    {
        // Use either height, width, or combination
        float baseSize = juce::jmin(bounds.getHeight(), bounds.getWidth());
        return baseSize * 0.25f; // 35% of smallest dimension
    }

    juce::Font getFontForButton(juce::TextButton& button, float size)
    {
        auto f = getTextButtonFont(button, button.getHeight());
        return f.withHeight(size);
    }
};