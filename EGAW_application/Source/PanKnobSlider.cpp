/*
  ==============================================================================

    PanKnobSlider.cpp
    Created: 5 Mar 2025 9:31:31am
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PanKnobSlider.h"

//==============================================================================
PanKnobSlider::PanKnobSlider(juce::ValueTree& channelSettings) 
    : juce::Slider(SliderStyle::RotaryHorizontalVerticalDrag, TextEntryBoxPosition::TextBoxBelow)
    , channelSettings(channelSettings)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    SignalManagerUI::getInstance().addListener(this);

    //DBG(channelSettings.getType().toString());
	setTooltip("Click to set pan");

    //setTextValueSuffix(" dB");
    setValue(channelSettings.getProperty("Pan"), juce::NotificationType::dontSendNotification);
    setSkewFactor(1, true); //What the hell??
    setRange(-1.0, 1.0, 0.01);
    setScrollWheelEnabled(true);
    setVelocityBasedMode(true);
    setVelocityModeParameters(2, 1);
    isTextActive = false;
    setDataMode(isTextActive);

}

PanKnobSlider::~PanKnobSlider()
{
    SignalManagerUI::getInstance().removeListener(this);
}

void PanKnobSlider::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced(15);
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

void PanKnobSlider::handleMessage(const juce::Message& message)
{
    if (auto* signalMsg = dynamic_cast<const SignalMessage*>(&message)) {
        auto signal = static_cast<SignalManagerUI::Signal>(signalMsg->getSignalType());
        // Handle signal (already on message thread)...

        switch (signal)
        {
        case SignalManagerUI::Signal::RESTORE_UI_PARAMETERS:
            setValue(static_cast<float>(channelSettings.getProperty("Pan")));
            break;
        default:
            break;
        }
    }
}

void PanKnobSlider::setDataMode(bool shouldBeOn)
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
    SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
}

void PanKnobSlider::mouseDrag(const juce::MouseEvent& event)
{
    //setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    setMouseCursor(juce::MouseCursor::NormalCursor);
    channelSettings.setProperty("Pan", getValue(), nullptr);
    juce::Slider::mouseDrag(event);
}

void PanKnobSlider::mouseDoubleClick(const juce::MouseEvent& event)
{
    setValue(0.0);
    channelSettings.setProperty("Pan", getValue(), nullptr);
    juce::Slider::mouseDoubleClick(event);
}

void PanKnobSlider::mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel)
{
    channelSettings.setProperty("Pan", getValue(), nullptr);
    juce::Slider::mouseWheelMove(event, wheel);
}

void PanKnobSlider::mouseDown(const juce::MouseEvent& event)
{
    // Check if the right mouse button was clicked
    if (event.mods.isRightButtonDown())
    {
        // Call your custom function here
        isTextActive = !isTextActive;
        setDataMode(isTextActive);
    }
    else
    {
        // Call the base class method for default left-click behavior
        //setMouseCursor(juce::MouseCursor::DraggingHandCursor);
        MixerInterfaceFocusSingleton::getInstance().setChannelUuid(channelSettings.getType().toString());
        MixerInterfaceFocusSingleton::getInstance().setState(MixerInterfaceFocusSingleton::MixerFocusState::FOCUS_ON);
        juce::Slider::mouseDown(event);
    }
}

void PanKnobSlider::mouseUp(const juce::MouseEvent& event)
{
	setMouseCursor(juce::MouseCursor::NormalCursor);
	MixerInterfaceFocusSingleton::getInstance().setState(MixerInterfaceFocusSingleton::MixerFocusState::FOCUS_OFF);
	juce::Slider::mouseUp(event);
}
