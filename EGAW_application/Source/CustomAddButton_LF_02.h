/*
  ==============================================================================

    CustomAddButton_LF_02.h
    Created: 13 May 2025 6:36:54pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "ColourConstants.h"

class CustomAddButton_LF_02 : public juce::LookAndFeel_V4
{
public:
    void drawButtonBackground(juce::Graphics& g,
        juce::Button& button,
        const juce::Colour& backgroundColour,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) override
    {
        auto bounds = button.getLocalBounds().toFloat();
        auto cutEdge = bounds.getHeight() / 20;
        auto borderThickness = 2.0f;

        juce::Path outerPath;

        // Create trapezoid shape with cut corners
        outerPath.startNewSubPath(0, 0);
        outerPath.lineTo(0, bounds.getBottom());
        outerPath.lineTo(bounds.getRight(), bounds.getBottom() - cutEdge);
        outerPath.lineTo(bounds.getRight(), cutEdge);
        outerPath.closeSubPath();

        auto outerColor = colourConstants::dark_green.darker(0.5f);
        g.setColour(outerColor);
        g.fillPath(outerPath);

        juce::Path innerPath;

        // Create trapezoid shape with cut corners
        innerPath.startNewSubPath(borderThickness, borderThickness);
        innerPath.lineTo(borderThickness, bounds.getBottom() - borderThickness);
        innerPath.lineTo(bounds.getRight() - borderThickness, bounds.getBottom() - cutEdge - borderThickness);
        innerPath.lineTo(bounds.getRight() - borderThickness, cutEdge + borderThickness);
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
        auto font = getFontForButton(button, fontSize);
        g.setFont(font);
        g.setColour(button.findColour(button.getToggleState() ?
            juce::TextButton::textColourOnId :
            juce::TextButton::textColourOffId));

        // Calculate available text area (with some padding)
        const float padding = bounds.getHeight() * 0.1f;
        const float textWidth = bounds.getWidth() - 2 * padding;

        // Break text into multiple lines if needed
        juce::StringArray lines;
        auto text = button.getButtonText();
        auto textNeedsWrapping = font.getStringWidth(text) > textWidth;

        if (textNeedsWrapping)
        {
            // Simple word wrapping - split at spaces
            auto words = juce::StringArray::fromTokens(text, " ", "");
            juce::String currentLine;

            for (auto& word : words)
            {
                if (font.getStringWidth(currentLine + word) <= textWidth)
                    currentLine += (currentLine.isEmpty() ? "" : " ") + word;
                else
                {
                    if (!currentLine.isEmpty())
                        lines.add(currentLine);
                    currentLine = word;
                }
            }

            if (!currentLine.isEmpty())
                lines.add(currentLine);
        }
        else
        {
            lines.add(text);
        }

        // Calculate vertical positioning
        const float lineHeight = font.getHeight();
        const float totalTextHeight = lines.size() * lineHeight;
        float y = bounds.getY() + (bounds.getHeight() - totalTextHeight) / 2;

        // Draw each line centered
        for (auto& line : lines)
        {
            g.drawText(line,
                bounds.getX(), y, bounds.getWidth(), lineHeight,
                juce::Justification::centred,
                false); // Don't use ellipsis
            y += lineHeight;
        }
    }

private:

    float calculateFontSize(const juce::Rectangle<float>& bounds)
    {
        // Use either height, width, or combination
        float baseSize = juce::jmin(bounds.getHeight(), bounds.getWidth());
        return baseSize * 0.20f; // 35% of smallest dimension
    }

    juce::Font getFontForButton(juce::TextButton& button, float size)
    {
        auto f = getTextButtonFont(button, button.getHeight());
        return f.withHeight(size);
    }
};