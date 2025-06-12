/*
  ==============================================================================

    PlayPauseButton.h
    Created: 8 May 2025 10:00:26pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ColourConstants.h"

class PlayPauseButton : public juce::Button
{
public:
    PlayPauseButton() : juce::Button("PlayPauseButton")
    {
        setClickingTogglesState(true);
        setWantsKeyboardFocus(true);
    }

    void paintButton(juce::Graphics& g, bool isMouseOver, bool isButtonDown) override
    {
        auto bounds = getLocalBounds().toFloat();

        // 1. Draw left-rounded background
        const float cornerRadius = bounds.getHeight() / 2.0f;
        juce::Path background;

        // Start at top-right (sharp corner)
        background.startNewSubPath(bounds.getBottomRight());

        // Draw clockwise:
        background.lineTo(bounds.getTopRight());
        background.lineTo(cornerRadius, 0);

        background.addCentredArc(cornerRadius,
            bounds.getHeight() / 2,
            cornerRadius, cornerRadius,
            0,
            juce::MathConstants<float>::pi * 0.0f,  // 12 o'clock
            juce::MathConstants<float>::pi * -1.0f);  // 6 o'clock

        background.closeSubPath();

        // Fill with state-aware color
        auto baseColor = colourConstants::dark_green.darker(0.5f);
        // if (isMouseOver) baseColor = baseColor.brighter(0.15f);
        g.setColour(baseColor);
        g.fillPath(background);


        const float borderThickness = 2.0f; // Adjust as needed
        juce::Path innerBackground;
        // Start at top-right (sharp corner)
        innerBackground.startNewSubPath(bounds.getBottomRight().getX() - borderThickness, bounds.getBottomRight().getY() - borderThickness);

        // Draw clockwise:
        innerBackground.lineTo(bounds.getTopRight().getX() - borderThickness, bounds.getTopRight().getY() + borderThickness);
        innerBackground.lineTo(cornerRadius + borderThickness, 0 + borderThickness);

        innerBackground.addCentredArc(cornerRadius,
            bounds.getHeight() / 2,
            cornerRadius - borderThickness, cornerRadius - borderThickness,
            0,
            juce::MathConstants<float>::pi * 0.0f,  // 12 o'clock
            juce::MathConstants<float>::pi * -1.0f);  // 6 o'clock

        innerBackground.closeSubPath();

        // Fill with state-aware color
        auto innerColor = colourConstants::dark_green;
        if (isMouseOver) innerColor = innerColor.brighter(0.15f);
        g.setColour(innerColor);
        g.fillPath(innerBackground);

        // 2. Draw centered icon (occupies 2/3 of height)
        const float iconHeight = bounds.getHeight() * 0.666f - borderThickness * 2;
        auto iconArea = bounds.withSizeKeepingCentre(iconHeight, iconHeight); // Square area

        g.setColour(juce::Colours::white);
        if (getToggleState())
        {
            // Pause state - two vertical bars (30% width each)
            const float barWidth = iconArea.getWidth() * 0.3f;
            const float gap = (iconArea.getWidth() - (barWidth * 2)) / 3.0f;

            g.fillRect(iconArea.getX() + gap, iconArea.getY(),
                barWidth, iconArea.getHeight());
            g.fillRect(iconArea.getRight() - gap - barWidth, iconArea.getY(),
                barWidth, iconArea.getHeight());
        }
        else
        {
            // Play state - equilateral triangle
            const float triangleHeight = iconArea.getHeight();
            const float triangleWidth = triangleHeight * 0.866f; // sqrt(3)/2 for equilateral

            juce::Path playTriangle;
            playTriangle.addTriangle(
                iconArea.getCentreX() - triangleWidth / 2, iconArea.getY(), // Left point
                iconArea.getCentreX() + triangleWidth / 2, iconArea.getCentreY(), // Right point
                iconArea.getCentreX() - triangleWidth / 2, iconArea.getBottom() // Bottom point
            );
            g.fillPath(playTriangle);
        }
    }

    void resized() override
    {
        // Ensure minimum size for visibility
        setSize(juce::jmax(getWidth(), 30),
            juce::jmax(getHeight(), 30));
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayPauseButton)
};