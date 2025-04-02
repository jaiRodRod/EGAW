/*
  ==============================================================================

    ChannelVolumeSlider.cpp
    Created: 4 Mar 2025 3:56:44pm
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ChannelVolumeSlider.h"

//==============================================================================
ChannelVolumeSlider::ChannelVolumeSlider(juce::ValueTree& channelSettings) 
    : juce::Slider(SliderStyle::LinearVertical, TextEntryBoxPosition::TextBoxBelow)
    , channelSettings(channelSettings)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    SignalManagerUI::getInstance().addListener(this);

    setTextValueSuffix(" dB");
    setValue(channelSettings.getProperty("Gain"), juce::NotificationType::dontSendNotification);
    setSkewFactor(1, true); //What the hell??
    setRange(-36.0, 18.0, 0.01);
    setScrollWheelEnabled(true);
    setVelocityBasedMode(true);
    setVelocityModeParameters(3.5, 1);
    isTextActive = false;
    setDataMode(isTextActive);

    //setChangeNotificationOnlyOnRelease() INTERESTING

}

ChannelVolumeSlider::~ChannelVolumeSlider()
{
    SignalManagerUI::getInstance().removeListener(this);
}

void ChannelVolumeSlider::handleMessage(const juce::Message& message)
{
    if (auto* signalMsg = dynamic_cast<const SignalMessage*>(&message)) {
        auto signal = static_cast<SignalManagerUI::Signal>(signalMsg->getSignalType());
        // Handle signal (already on message thread)...

        switch (signal)
        {
        case SignalManagerUI::Signal::RESTORE_UI_PARAMETERS:
            setValue(static_cast<double>(channelSettings.getProperty("Gain")));
            break;
        default:
            break;
        }
    }
}

void ChannelVolumeSlider::setDataMode(bool shouldBeOn)
{
    if (shouldBeOn)
    {
        setTextValueSuffix(" dB");
        setTextBoxStyle(TextBoxBelow, false, getTextBoxWidth(), getTextBoxHeight());
    }
    else
    {
        setTextBoxStyle(NoTextBox, true, getTextBoxWidth(), getTextBoxHeight());
    }
    SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
}

void ChannelVolumeSlider::mouseDrag(const juce::MouseEvent& event)
{
    channelSettings.setProperty("Gain", getValue(), nullptr);
    juce::Slider::mouseDrag(event);
}

void ChannelVolumeSlider::mouseDoubleClick(const juce::MouseEvent& event)
{
    setValue(0.0);
    channelSettings.setProperty("Gain", getValue(), nullptr);
    juce::Slider::mouseDoubleClick(event);
}

void ChannelVolumeSlider::mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel)
{
    channelSettings.setProperty("Gain", getValue(), nullptr);
    juce::Slider::mouseWheelMove(event, wheel);
}

void ChannelVolumeSlider::mouseDown(const juce::MouseEvent& event)
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
        juce::Slider::mouseDown(event);
    }
}
