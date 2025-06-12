/*
  ==============================================================================

    DrySlider.cpp
    Created: 27 May 2025 5:29:08pm
    Author:  jaezro

  ==============================================================================
*/

#include "DrySlider.h"

DrySlider::DrySlider() : dryLabel("DryLabel", "Dry Level"), showingLevel(false)
{
    dryLabel.setText("Dry Level", juce::dontSendNotification);
    dryLabel.attachToComponent(this, false);
    dryLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(dryLabel);

	setLookAndFeel(&verticalFaderLookAndFeel);

	setTooltip("Dry Signal Level");

    setSliderStyle(juce::Slider::LinearVertical);
    setTextBoxStyle(NoTextBox, true, getTextBoxWidth(), getTextBoxHeight());
    //setTextValueSuffix(" dB");
    setRange(0.0, 1.0, 0.01); // Typical range for volume sliders
    setValue(0.0); // Default value
    setScrollWheelEnabled(true);
    setVelocityBasedMode(true);
    setVelocityModeParameters(3.5, 1);
}

DrySlider::~DrySlider()
{
	setLookAndFeel(nullptr);
}

void DrySlider::setDataMode(bool isVisible)
{
    if (isVisible)
    {
        //setTextValueSuffix(" dB");
        setTextBoxStyle(TextBoxBelow, false, getTextBoxWidth(), getTextBoxHeight());
    }
    else
    {
        setTextBoxStyle(NoTextBox, true, getTextBoxWidth(), getTextBoxHeight());
    }
}

void DrySlider::mouseDrag(const juce::MouseEvent& event)
{
    setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    juce::Slider::mouseDrag(event);
}

void DrySlider::mouseDoubleClick(const juce::MouseEvent& event)
{
    setValue(0.0);
    juce::Slider::mouseDoubleClick(event);
}

void DrySlider::mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel)
{
    juce::Slider::mouseWheelMove(event, wheel);
}

void DrySlider::mouseDown(const juce::MouseEvent& event)
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

void DrySlider::mouseUp(const juce::MouseEvent& event)
{
    setMouseCursor(juce::MouseCursor::NormalCursor);
    juce::Slider::mouseUp(event);
}
