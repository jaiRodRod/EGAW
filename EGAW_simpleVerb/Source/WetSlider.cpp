/*
  ==============================================================================

    WetSlider.cpp
    Created: 27 May 2025 5:29:16pm
    Author:  jaezro

  ==============================================================================
*/

#include "WetSlider.h"

WetSlider::WetSlider() : wetLabel("WetLabel", "Wet Level"), showingLevel(false)
{
    wetLabel.setText("Wet Level", juce::dontSendNotification);
    wetLabel.attachToComponent(this, false);
    wetLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(wetLabel);

    setLookAndFeel(&verticalFaderLookAndFeel);

    setTooltip("Wet Signal Level");

    setSliderStyle(juce::Slider::LinearVertical);
    setTextBoxStyle(NoTextBox, true, getTextBoxWidth(), getTextBoxHeight());
    //setTextValueSuffix(" dB");
    setRange(0.0, 1.0, 0.01); // Typical range for volume sliders
    setValue(0.0); // Default value
    setScrollWheelEnabled(true);
    setVelocityBasedMode(true);
    setVelocityModeParameters(3.5, 1);
}

WetSlider::~WetSlider()
{
	setLookAndFeel(nullptr);
}

void WetSlider::setDataMode(bool isVisible)
{
    if (isVisible)
    {
        //setTextValueSuffix("");
        setTextBoxStyle(TextBoxBelow, false, getTextBoxWidth(), getTextBoxHeight());
    }
    else
    {
        setTextBoxStyle(NoTextBox, true, getTextBoxWidth(), getTextBoxHeight());
    }
}

void WetSlider::mouseDrag(const juce::MouseEvent& event)
{
    setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    juce::Slider::mouseDrag(event);
}

void WetSlider::mouseDoubleClick(const juce::MouseEvent& event)
{
    setValue(0.0);
    juce::Slider::mouseDoubleClick(event);
}

void WetSlider::mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel)
{
    juce::Slider::mouseWheelMove(event, wheel);
}

void WetSlider::mouseDown(const juce::MouseEvent& event)
{
    if (event.mods.isRightButtonDown())
    {
        // Call your custom function here
        showingLevel = !showingLevel;
        setDataMode(showingLevel);
    }
    else
    {
        // Call the base class method for default left-click behavior
        setMouseCursor(juce::MouseCursor::DraggingHandCursor);
        juce::Slider::mouseDown(event);
    }
}

void WetSlider::mouseUp(const juce::MouseEvent& event)
{
    setMouseCursor(juce::MouseCursor::NormalCursor);
    juce::Slider::mouseUp(event);
}
