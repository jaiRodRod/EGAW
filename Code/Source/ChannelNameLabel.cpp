/*
  ==============================================================================

    ChannelNameLabel.cpp
    Created: 7 Mar 2025 9:45:06am
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ChannelNameLabel.h"

ChannelNameLabel::ChannelNameLabel(juce::ValueTree& channelSettings)
    : juce::Label(),
    channelSettings(channelSettings)
{
    // Changed to direct MessageListener registration
    SignalManagerUI::getInstance().addListener(this);

    onTextChange = [this] { textChange(); };
    setEditable(false, true, false);
    setText(channelSettings.getProperty("Name"), juce::NotificationType::dontSendNotification);
}

ChannelNameLabel::~ChannelNameLabel()
{
    SignalManagerUI::getInstance().removeListener(this);
}

void ChannelNameLabel::handleMessage(const juce::Message& msg)
{
    // Safe dynamic cast to our custom message type
    if (const auto* signalMsg = dynamic_cast<const SignalMessage*>(&msg))
    {
        const auto signal = static_cast<SignalManagerUI::Signal>(signalMsg->getSignalType());

        switch (signal)
        {
        case SignalManagerUI::Signal::RESTORE_UI_PARAMETERS:
            setText(channelSettings.getProperty("Name"),
                juce::NotificationType::dontSendNotification);
            break;
        default:
            break;
        }
    }
}

void ChannelNameLabel::textChange()
{
    channelSettings.setProperty("Name", getText(), nullptr);
}