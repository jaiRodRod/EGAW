/*
  ==============================================================================

    CustomMoveButton_LF_02.h
    Created: 13 May 2025 7:56:35pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CustomMoveButton_LF_Left : public juce::LookAndFeel_V4
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

        // Calculate triangle points (equilateral with flat side on right)
        float padding = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.15f;
        float size = juce::jmin(bounds.getWidth(), bounds.getHeight()) - padding * 2;

        juce::Point<float> rightTop(bounds.getRight() - padding,
            bounds.getCentreY() - size * 0.433f);
        juce::Point<float> rightBottom(bounds.getRight() - padding,
            bounds.getCentreY() + size * 0.433f);
        juce::Point<float> leftCenter(bounds.getRight() - padding - size * 0.866f,
            bounds.getCentreY());

        juce::Path triangle;
        triangle.startNewSubPath(rightTop);
        triangle.lineTo(rightBottom);
        triangle.lineTo(leftCenter);
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