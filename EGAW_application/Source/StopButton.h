/*
  ==============================================================================

    StopButton.h
    Created: 8 May 2025 11:00:47pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ColourConstants.h"

class StopButton : public juce::Button
{
public:
    StopButton() : juce::Button("StopButton") {
        setClickingTogglesState(true);
        setWantsKeyboardFocus(true);
    }

    void paintButton(juce::Graphics& g, bool isMouseOver, bool isButtonDown) override
    {
        auto bounds = getLocalBounds().toFloat();

        const float cornerRadius = bounds.getHeight() / 2.0f;

        // 1. Create path with perfect right-only rounding
        juce::Path background;
		background.setUsingNonZeroWinding(false); // Non-zero winding rule for better performance

        // Start at bottom-left (sharp corner)
        background.startNewSubPath(0,bounds.getBottom());

        // Bottom edge (left to right) - stops before curve
        background.lineTo(bounds.getRight() - cornerRadius, bounds.getBottom());

        // Bottom-right rounded corner (270° to 360°)
        background.addCentredArc(bounds.getRight() - cornerRadius ,
            bounds.getHeight() / 2,
            cornerRadius , cornerRadius ,
            0,
            juce::MathConstants<float>::pi * 1.0f,  // 6 o'clock
            juce::MathConstants<float>::pi * 0.0f);  // 12 o'clock

		background.lineTo(bounds.getRight() - cornerRadius, 0);

        // Top edge (right to left)
        background.lineTo(0,0);

        // Left edge (top to bottom) - completes the rectangle
        background.closeSubPath();

        // 2. Fill background
		auto baseColor = colourConstants::dark_green.darker(0.5f);
        // if (isMouseOver) baseColor = baseColor.brighter(0.15f);
        g.setColour(baseColor);
        g.fillPath(background);

        const float borderThickness = 2.0f; // Adjust as needed
        juce::Path innerBackground;
        innerBackground.setUsingNonZeroWinding(false); // Non-zero winding rule for better performance

        // Start at bottom-left (sharp corner)
        innerBackground.startNewSubPath(0 + borderThickness, bounds.getBottom() - borderThickness);

        // Bottom edge (left to right) - stops before curve
        innerBackground.lineTo(bounds.getRight() - cornerRadius - borderThickness, bounds.getBottom() - borderThickness);

        // Bottom-right rounded corner (270° to 360°)
        innerBackground.addCentredArc(bounds.getRight() - cornerRadius,
            bounds.getHeight() / 2,
            cornerRadius - borderThickness, cornerRadius - borderThickness,
            0,
            juce::MathConstants<float>::pi * 1.0f,  // 6 o'clock
            juce::MathConstants<float>::pi * 0.0f);  // 12 o'clock

        innerBackground.lineTo(bounds.getRight() - cornerRadius - borderThickness, 0 + borderThickness);

        // Top edge (right to left)
        innerBackground.lineTo(0 + borderThickness, 0 + borderThickness);

        // Left edge (top to bottom) - completes the rectangle
        innerBackground.closeSubPath();

        // 2. Fill background
        auto innerColor = colourConstants::dark_green;
        if (isMouseOver) innerColor = innerColor.brighter(0.15f);
        g.setColour(innerColor);
        g.fillPath(innerBackground);

        // 3. Draw centered square icon (50% height)
        const float squareSize = bounds.getHeight() * 0.5f;
        auto squareBounds = bounds.withSizeKeepingCentre(squareSize, squareSize);
        g.setColour(juce::Colours::white);
        g.fillRect(squareBounds);
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StopButton)
};