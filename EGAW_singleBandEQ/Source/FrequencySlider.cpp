/*
  ==============================================================================

    FrequencySlider.cpp
    Created: 26 May 2025 3:44:13pm
    Author:  jaezro

  ==============================================================================
*/

#include "FrequencySlider.h"


FrequencySlider::FrequencySlider() : isTextActive(false)
{
    setTooltip("Frequency Control");

    setLookAndFeel(&lookAndFeel);

    setSliderStyle(juce::Slider::LinearHorizontal);
    setTextBoxStyle(NoTextBox, true, getTextBoxWidth(), getTextBoxHeight());
    //setTextValueSuffix(" dB");
    setRange(20.0, 20000.0, 0.01); // Typical range for volume sliders
    setValue(0.0); // Default value
    setScrollWheelEnabled(true);
    setVelocityBasedMode(true);
    setVelocityModeParameters(3.5, 1);
}

FrequencySlider::~FrequencySlider()
{
    setLookAndFeel(nullptr); // Reset the look and feel to default
    // This is important to avoid dangling pointers if the look and feel is deleted
    // before this slider.
}

void FrequencySlider::setDataMode(bool isVisible)
{
    if (isVisible)
    {
        setTextValueSuffix(" Hz");
        setTextBoxStyle(TextBoxBelow, false, getTextBoxWidth(), getTextBoxHeight());
    }
    else
    {
        setTextBoxStyle(NoTextBox, true, getTextBoxWidth(), getTextBoxHeight());
    }
}

void FrequencySlider::mouseDrag(const juce::MouseEvent& event)
{
    setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    juce::Slider::mouseDrag(event);
}

void FrequencySlider::mouseDoubleClick(const juce::MouseEvent& event)
{
    setValue(0.0);
    juce::Slider::mouseDoubleClick(event);
}

void FrequencySlider::mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel)
{
    juce::Slider::mouseWheelMove(event, wheel);
}

void FrequencySlider::mouseDown(const juce::MouseEvent& event)
{
    if (event.mods.isRightButtonDown())
    {
        // Call your custom function here
        isTextActive = !isTextActive;
        setDataMode(isTextActive);
    }
    else
    {
        // Call the base class method for default left-click behavior
        setMouseCursor(juce::MouseCursor::DraggingHandCursor);
        juce::Slider::mouseDown(event);
    }
}

void FrequencySlider::mouseUp(const juce::MouseEvent& event)
{
    setMouseCursor(juce::MouseCursor::NormalCursor);
    juce::Slider::mouseUp(event);
}
