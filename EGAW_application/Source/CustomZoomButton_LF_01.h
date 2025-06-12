/*
  ==============================================================================

    CustomZoomButton_LF_01.h
    Created: 10 May 2025 4:32:16pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ColourConstants.h"

class CustomZoomButton_LF : public juce::LookAndFeel_V4
{
public:
    CustomZoomButton_LF(bool isTopRounded)
        : topRounded(isTopRounded) {
    }

    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
        const juce::Colour& backgroundColour,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) override
    {
        auto cornerSize = 6.0f;
        auto bounds = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);

        // Only round the top or bottom corners
        if (topRounded)
        {
            bounds = bounds.withBottom(bounds.getBottom() + cornerSize);
        }
        else
        {
            bounds = bounds.withTop(0 - cornerSize);
        }

        auto baseColor = colourConstants::dark_green;
        g.setColour(baseColor.withAlpha(button.isEnabled() ? 1.0f : 0.5f));
        g.fillRoundedRectangle(bounds, cornerSize);
    }

    // In your LookAndFeel class (or add this to existing ones)
    void drawButtonText(juce::Graphics& g, juce::TextButton& button,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) override
    {
        auto font = juce::Font(juce::Font::getDefaultSansSerifFontName(),
            button.getHeight() * 0.8f,
            juce::Font::bold);
        g.setFont(font);
        g.setColour(button.findColour(button.getToggleState() ? juce::TextButton::textColourOnId
            : juce::TextButton::textColourOffId)
            .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f));

        const int yIndent = juce::jmin(4, button.proportionOfHeight(0.3f));
        const int cornerSize = juce::jmin(button.getHeight(), button.getWidth()) / 2;

        const int fontHeight = juce::roundToInt(font.getHeight() * 0.6f);
        const int leftIndent = juce::jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
        const int rightIndent = juce::jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
        const int textWidth = button.getWidth() - leftIndent - rightIndent;

        g.drawFittedText(button.getButtonText(),
            leftIndent, yIndent,
            textWidth, button.getHeight() - yIndent * 2,
            juce::Justification::centred,
            2);
    }

private:
    bool topRounded;
};