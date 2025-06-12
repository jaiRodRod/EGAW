/*
  ==============================================================================

    FrequencySlider_LF.h
    Created: 26 May 2025 4:56:01pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class FrequencySlider_LF : public juce::LookAndFeel_V4
{
public:
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const juce::Slider::SliderStyle style, juce::Slider& slider) override
    {
        if (style == juce::Slider::LinearHorizontal)
        {
            // Custom colors
            auto trackColor = juce::Colours::black;
            auto thumbColor = juce::Colours::black;
            auto stripeColor = juce::Colours::white;

            // Add padding to prevent thumb clipping
            const float padding = height * 0.15f;
            const float paddedX = x + padding;
            const float paddedWidth = width - 2 * padding;

            // Adjust slider positions to account for padding
            const float adjustedMinPos = paddedX;
            const float adjustedMaxPos = paddedX + paddedWidth;
            const float adjustedSliderPos = juce::jlimit(adjustedMinPos, adjustedMaxPos, sliderPos);

            // Draw the track/rail (thin black line)
            float trackHeight = height * 0.1f;  // 10% of height
            float trackTop = y + (height - trackHeight) * 0.5f;

            // Track background (full length)
            g.setColour(trackColor.withAlpha(0.3f));
            g.fillRect(paddedX, trackTop, paddedWidth, trackHeight);

            // Draw the thumb/fader (rounded rectangle with white stripe)
            float thumbHeight = height * 0.5f; // 60% of height
            float thumbWidth = thumbHeight * 0.4f;  // 80% of height (square-ish)
            float thumbTop = y + (height - thumbHeight) * 0.5f;
            float thumbLeft = x + (width - thumbWidth) * 0.5f;

            // Draw thumb body
            g.setColour(thumbColor);
            g.fillRoundedRectangle(adjustedSliderPos - thumbWidth * 0.5f, thumbTop,
                thumbWidth, thumbHeight, 2.0f);

            /*
            DBG("-------------------");
            DBG("Adjusted slider pos: " << adjustedSliderPos);
			DBG("Thumb top: " << thumbTop);
			DBG("Thumb width: " << thumbWidth);
			DBG("Thumb height: " << thumbHeight);
            */

            auto baseColour = 255;

            juce::ColourGradient gradient(
                juce::Colour(baseColour * 0.6f, baseColour * 0.6f, baseColour * 0.6f), adjustedSliderPos - thumbWidth * 0.5f, thumbTop,
                juce::Colour(baseColour * 0.6f, baseColour * 0.6f, baseColour * 0.6f), adjustedSliderPos + thumbWidth * 0.5f, thumbTop, false);

            // Add carefully spaced color stops
            gradient.addColour(0.0f, juce::Colour(baseColour * 0.6f, baseColour * 0.6f, baseColour * 0.6f));          // Bottom
            gradient.addColour(0.2f, juce::Colour(baseColour * 0.5f, baseColour * 0.5f, baseColour * 0.5f)); // Slow start
            gradient.addColour(0.3f, juce::Colour(baseColour * 0.4f, baseColour * 0.4f, baseColour * 0.4f)); // Faster change
            gradient.addColour(0.45f, juce::Colour(baseColour * 0.25f, baseColour * 0.25f, baseColour * 0.25f)); // Slowing down
			gradient.addColour(0.5f, juce::Colour(baseColour * 0.1f, baseColour * 0.1f, baseColour * 0.1f)); // Slowing down
            gradient.addColour(0.55f, juce::Colour(baseColour * 0.25f, baseColour * 0.25f, baseColour * 0.25f)); // Slowing down
            gradient.addColour(0.7f, juce::Colour(baseColour * 0.4f, baseColour * 0.4f, baseColour * 0.4f)); // Faster change
            gradient.addColour(0.8f, juce::Colour(baseColour * 0.5f, baseColour * 0.5f, baseColour * 0.5f)); // Fast end
            gradient.addColour(1.0f, juce::Colour(baseColour * 0.6f, baseColour * 0.6f, baseColour * 0.6f));          // Top


            g.setGradientFill(gradient);

            g.fillRoundedRectangle(adjustedSliderPos - thumbWidth * 0.5f, thumbTop, thumbWidth, thumbHeight, 2.0f);


            // Draw thumb body
            /*
            g.setColour(juce::Colours::grey);
            g.drawRoundedRectangle(adjustedSliderPos - thumbWidth * 0.5f, thumbTop,
                thumbWidth, thumbHeight, 2.0f, 2.0f);
            */

            // Draw white stripe in the middle
            float stripeWidth = 3;
            float stripeHeight = thumbHeight;
            g.setColour(stripeColor);
            g.fillRect(adjustedSliderPos - stripeWidth * 0.5f,
                y + (height - stripeHeight) * 0.5f,
                stripeWidth, stripeHeight);
        }
        else
        {
            // Fallback for other slider styles
            LookAndFeel_V4::drawLinearSlider(g, x, y, width, height, sliderPos,
                minSliderPos, maxSliderPos, style, slider);
        }
    }
};