/*
  ==============================================================================

    CustomMoveButton_LF_01.h
    Created: 13 May 2025 7:45:47pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CustomMoveButton_LF_Right : public juce::LookAndFeel_V4
{
public:
    void drawButtonBackground(juce::Graphics& g,
        juce::Button& button,
        const juce::Colour& backgroundColour,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) override
    {
        g.fillAll(juce::Colours::transparentBlack);
    }

    void drawButtonText(juce::Graphics& g,
        juce::TextButton& button,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) override
    {
        auto bounds = button.getLocalBounds().toFloat().reduced(2);
        auto triangleColour = juce::Colours::black//button.findColour(juce::TextButton::textColourOffId)
            .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f);

        // Calculate triangle points (equilateral with flat side on left)
        float padding = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.15f;
        float size = juce::jmin(bounds.getWidth(), bounds.getHeight()) - padding * 2;

        juce::Point<float> leftTop(bounds.getX() + padding,
            bounds.getCentreY() - size * 0.433f); // 0.433 = 0.866/2
        juce::Point<float> leftBottom(bounds.getX() + padding,
            bounds.getCentreY() + size * 0.433f);
        juce::Point<float> rightCenter(bounds.getX() + padding + size * 0.866f,
            bounds.getCentreY());

        juce::Path triangle;
        triangle.startNewSubPath(leftTop);
        triangle.lineTo(leftBottom);
        triangle.lineTo(rightCenter);
        triangle.closeSubPath();

        g.setColour(triangleColour);
        g.fillPath(triangle);

        if (shouldDrawButtonAsHighlighted)
        {
            g.setColour(triangleColour.withAlpha(0.3f));
            g.strokePath(triangle, juce::PathStrokeType(2.0f));
        }
    }
};