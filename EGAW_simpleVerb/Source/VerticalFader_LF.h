/*
  ==============================================================================

    VerticalFader_LF.h
    Created: 27 May 2025 5:29:36pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class VerticalFader_LF : public juce::LookAndFeel_V4
{
public:
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const juce::Slider::SliderStyle style, juce::Slider& slider) override
    {
        // Custom colors
        /*
        auto trackColor = slider.findColour(juce::Slider::trackColourId);
        auto thumbColor = slider.findColour(juce::Slider::thumbColourId);
        */

        auto trackColor = juce::Colours::black;
        auto thumbColor = juce::Colours::black;

        // For vertical slider
        if (style == juce::Slider::LinearVertical)
        {
            // Add padding to prevent thumb clipping (10% of height)
            const float padding = height * 0.08f;
            const float paddedY = y + padding;
            const float paddedHeight = height - 2 * padding;

            // Adjust slider positions to account for padding

            const float adjustedMinPos = paddedY;// +padding;
            const float adjustedMaxPos = paddedY + paddedHeight;// -padding;
            const float adjustedSliderPos = juce::jlimit(adjustedMinPos, adjustedMaxPos, sliderPos);
            const float neutralPos = adjustedMaxPos; //minSliderPos

            // Draw the track/rail
            float trackWidth = width * 0.15f;
            float trackLeft = x + (width - trackWidth) * 0.5f;

            // Track background (inactive portion)
            g.setColour(trackColor.withAlpha(0.3f));
            g.fillRect(trackLeft, paddedY, trackWidth, paddedHeight);

            // Track active portion (above the thumb for vertical)
            if (neutralPos - adjustedSliderPos > 0)
            {
                g.setColour(trackColor);
                g.fillRect(trackLeft, adjustedSliderPos, trackWidth, neutralPos - adjustedSliderPos);
            }


            // Draw the thumb/fader
            float thumbWidth = width * 0.3f;
            float thumbHeight = thumbWidth * 1.8f;
            float thumbLeft = x + (width - thumbWidth) * 0.5f;

            float lineHeight = thumbHeight / 16;

            float topBottomBorderHeight = thumbHeight / 8;

            g.setColour(thumbColor);
            g.fillRect(thumbLeft, adjustedSliderPos - thumbHeight * 0.5f,
                thumbWidth, thumbHeight);

            //-----------------------------------
            juce::ColourGradient gradientMid(
                juce::Colour(255 * 0.1f, 255 * 0.1f, 255 * 0.1f), thumbLeft, adjustedSliderPos - thumbHeight * 0.5f + topBottomBorderHeight,
                juce::Colour(255 * 0.9f, 255 * 0.9f, 255 * 0.9f), thumbLeft, adjustedSliderPos + thumbHeight * 0.5f - topBottomBorderHeight, false);

            // Add carefully spaced color stops
            gradientMid.addColour(0.0f, juce::Colour(255 * 0.25f, 255 * 0.25f, 255 * 0.25f));          // Bottom
            gradientMid.addColour(0.2f, juce::Colour(255 * 0.3f, 255 * 0.3f, 255 * 0.3f)); // Slow start
            gradientMid.addColour(0.3f, juce::Colour(255 * 0.35f, 255 * 0.35f, 255 * 0.35f)); // Faster change
            gradientMid.addColour(0.4f, juce::Colour(255 * 0.4f, 255 * 0.4f, 255 * 0.4f)); // Slowing down
            gradientMid.addColour(0.6f, juce::Colour(255 * 0.6f, 255 * 0.6f, 255 * 0.6f)); // Slowing down
            gradientMid.addColour(0.7f, juce::Colour(255 * 0.7f, 255 * 0.7f, 255 * 0.7f)); // Faster change
            gradientMid.addColour(0.8f, juce::Colour(255 * 0.85f, 255 * 0.85f, 255 * 0.85f)); // Fast end
            gradientMid.addColour(1.0f, juce::Colour(255 * 0.9f, 255 * 0.9f, 255 * 0.9f));          // Top


            g.setGradientFill(gradientMid);

            g.fillRect(thumbLeft, adjustedSliderPos - thumbHeight * 0.5f + topBottomBorderHeight, thumbWidth, thumbHeight - topBottomBorderHeight);

            //----------------------------------

            //-----------------------------------
            juce::ColourGradient gradientTop(
                juce::Colour(255 * 0.1f, 255 * 0.1f, 255 * 0.1f), thumbLeft, adjustedSliderPos - thumbHeight * 0.5f,
                juce::Colour(255 * 0.1f, 255 * 0.1f, 255 * 0.1f), thumbLeft, adjustedSliderPos - thumbHeight * 0.5f + topBottomBorderHeight, false);

            // Add carefully spaced color stops
            gradientTop.addColour(0.0f, juce::Colour(255 * 0.2f, 255 * 0.2f, 255 * 0.2f));          // Bottom
            gradientTop.addColour(0.2f, juce::Colour(255 * 0.3f, 255 * 0.3f, 255 * 0.3f)); // Slow start
            gradientTop.addColour(0.3f, juce::Colour(255 * 0.4f, 255 * 0.4f, 255 * 0.4f)); // Faster change
            gradientTop.addColour(0.4f, juce::Colour(255 * 0.5f, 255 * 0.5f, 255 * 0.5f)); // Slowing down
            gradientTop.addColour(0.6f, juce::Colour(255 * 0.6f, 255 * 0.6f, 255 * 0.6f)); // Slowing down
            gradientTop.addColour(0.7f, juce::Colour(255 * 0.7f, 255 * 0.7f, 255 * 0.7f)); // Faster change
            gradientTop.addColour(0.8f, juce::Colour(255 * 0.85f, 255 * 0.85f, 255 * 0.85f)); // Fast end
            gradientTop.addColour(0.9f, juce::Colour(255 * 0.9f, 255 * 0.9f, 255 * 0.9f));          // Top
            gradientTop.addColour(1.0f, juce::Colour(255 * 0.1f, 255 * 0.1f, 255 * 0.1f));          // Top

            g.setGradientFill(gradientTop);

            g.fillRect(thumbLeft, adjustedSliderPos - thumbHeight * 0.5f, thumbWidth, topBottomBorderHeight);

            //----------------------------------

            //-----------------------------------
            juce::ColourGradient gradientBottom(
                juce::Colour(255 * 0.9f, 255 * 0.9f, 255 * 0.9f), thumbLeft, adjustedSliderPos + thumbHeight * 0.5f - topBottomBorderHeight,
                juce::Colour(255 * 0.9f, 255 * 0.9f, 255 * 0.9f), thumbLeft, adjustedSliderPos + thumbHeight * 0.5f, false);

            // Add carefully spaced color stops
            gradientBottom.addColour(0.0f, juce::Colour(255 * 0.9f, 255 * 0.9f, 255 * 0.9f));
            gradientBottom.addColour(0.1f, juce::Colour(255 * 0.3f, 255 * 0.3f, 255 * 0.3f));          // Bottom
            gradientBottom.addColour(0.2f, juce::Colour(255 * 0.35f, 255 * 0.35f, 255 * 0.35f)); // Slow start
            gradientBottom.addColour(0.3f, juce::Colour(255 * 0.4f, 255 * 0.4f, 255 * 0.4f)); // Faster change
            gradientBottom.addColour(0.4f, juce::Colour(255 * 0.45f, 255 * 0.45f, 255 * 0.45f)); // Slowing down
            gradientBottom.addColour(0.6f, juce::Colour(255 * 0.5f, 255 * 0.5f, 255 * 0.5f)); // Slowing down
            gradientBottom.addColour(0.7f, juce::Colour(255 * 0.6f, 255 * 0.6f, 255 * 0.6f)); // Faster change
            gradientBottom.addColour(0.9f, juce::Colour(255 * 0.65f, 255 * 0.65f, 255 * 0.65f)); // Fast end
            gradientBottom.addColour(1.0f, juce::Colour(255 * 0.8f, 255 * 0.8f, 255 * 0.8f));          // Top


            g.setGradientFill(gradientBottom);

            g.fillRect(thumbLeft, adjustedSliderPos + thumbHeight * 0.5f - topBottomBorderHeight, thumbWidth, topBottomBorderHeight);

            //----------------------------------
            /*
            g.setColour(juce::Colours::white);
            g.drawRect(thumbLeft, adjustedSliderPos - thumbHeight * 0.5f,
                thumbWidth, thumbHeight, 2.0f);
            */

            // Optional: Add a highlight to the thumb
            /*
            g.setColour(thumbColor.brighter(0.3f));
            g.drawRoundedRectangle(thumbLeft, adjustedSliderPos - thumbHeight * 0.5f,
                thumbWidth, thumbHeight, 4.0f, 1.0f);
            */


            g.setColour(juce::Colours::white);
            g.fillRect(thumbLeft, adjustedSliderPos - lineHeight * 0.5f,
                thumbWidth, lineHeight);
        }
    }
};