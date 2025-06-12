/*
  ==============================================================================

    VolumeSlider.cpp
    Created: 26 May 2025 3:43:57pm
    Author:  jaezro

  ==============================================================================
*/

#include "VolumeSlider.h"

VolumeSlider::VolumeSlider() : isTextActive(false)
{
    setTooltip("Volume Control");

	setLookAndFeel(&lookAndFeel);

    setSliderStyle(juce::Slider::LinearVertical);
    setTextBoxStyle(NoTextBox, true, getTextBoxWidth(), getTextBoxHeight());
    //setTextValueSuffix(" dB");
    setRange(-24.0, 24.0, 0.01); // Typical range for volume sliders
    setValue(0.0); // Default value
    setScrollWheelEnabled(true);
    setVelocityBasedMode(true);
    setVelocityModeParameters(3.5, 1);
}

VolumeSlider::~VolumeSlider()
{
	setLookAndFeel(nullptr); // Reset the look and feel to default
	// This is important to avoid dangling pointers if the look and feel is deleted
	// before this slider.
}

void VolumeSlider::setDataMode(bool isVisible)
{
    if (isVisible)
    {
        setTextValueSuffix(" dB");
        setTextBoxStyle(TextBoxBelow, false, getTextBoxWidth(), getTextBoxHeight());
    }
    else
    {
        setTextBoxStyle(NoTextBox, true, getTextBoxWidth(), getTextBoxHeight());
    }
}

void VolumeSlider::mouseDrag(const juce::MouseEvent& event)
{
    setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    juce::Slider::mouseDrag(event);
}

void VolumeSlider::mouseDoubleClick(const juce::MouseEvent& event)
{
    setValue(0.0);
    juce::Slider::mouseDoubleClick(event);
}

void VolumeSlider::mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel)
{
    juce::Slider::mouseWheelMove(event, wheel);
}

void VolumeSlider::mouseDown(const juce::MouseEvent& event)
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

void VolumeSlider::mouseUp(const juce::MouseEvent& event)
{
    setMouseCursor(juce::MouseCursor::NormalCursor);
    juce::Slider::mouseUp(event);
}
