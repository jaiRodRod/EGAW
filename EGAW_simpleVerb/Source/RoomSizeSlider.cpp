/*
  ==============================================================================

    RoomSizeSlider.cpp
    Created: 27 May 2025 6:28:37pm
    Author:  jaezro

  ==============================================================================
*/

#include "RoomSizeSlider.h"

RoomSizeSlider::RoomSizeSlider() : juce::Slider(SliderStyle::RotaryHorizontalVerticalDrag, TextEntryBoxPosition::TextBoxBelow), isTextActive(false)
{
	//setSliderStyle(juce::Slider::LinearVertical);
	setTextBoxStyle(NoTextBox, true, getTextBoxWidth(), getTextBoxHeight());
    setSkewFactor(1, true); //What the hell??
	setScrollWheelEnabled(true);
	setVelocityBasedMode(true);
	setVelocityModeParameters(2, 1);
	setRange(0.0, 1.0, 0.01); // Typical range for room size
	setValue(0.25); // Default value
}

RoomSizeSlider::~RoomSizeSlider()
{
}

void RoomSizeSlider::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced(5);
    auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto center = bounds.getCentre();
    auto angle = juce::degreesToRadians((180.0f + (getValue() - getMinimum()) / (getMaximum() - getMinimum()) * 270.0f) + 45.0f);

    // Draw the background circle with rugged border
    g.setColour(juce::Colours::black);
    g.fillEllipse(center.x - radius, center.y - radius, radius * 2.0f, radius * 2.0f);

    // Draw the pointer line
    juce::Path pointer;
    auto pointerLength = radius * 0.7f;
    auto pointerThickness = 4.0f;
    pointer.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);

    g.setColour(juce::Colours::white);
    g.fillPath(pointer, juce::AffineTransform::rotation(angle).translated(center));
}

void RoomSizeSlider::setDataMode(bool shouldBeOn)
{
    if (shouldBeOn)
    {
        //setTextValueSuffix(" pan");
        setTextBoxStyle(TextBoxBelow, false, getTextBoxWidth(), getTextBoxHeight());
    }
    else
    {
        setTextBoxStyle(NoTextBox, true, getTextBoxWidth(), getTextBoxHeight());
    }
}

void RoomSizeSlider::mouseDrag(const juce::MouseEvent& event)
{
    setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    juce::Slider::mouseDrag(event);
}

void RoomSizeSlider::mouseDoubleClick(const juce::MouseEvent& event)
{
    setValue(0.0f);
    juce::Slider::mouseDoubleClick(event);
}

void RoomSizeSlider::mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel)
{
    juce::Slider::mouseWheelMove(event, wheel);
}

void RoomSizeSlider::mouseDown(const juce::MouseEvent& event)
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

void RoomSizeSlider::mouseUp(const juce::MouseEvent& event)
{
    setMouseCursor(juce::MouseCursor::NormalCursor);
    juce::Slider::mouseUp(event);
}
