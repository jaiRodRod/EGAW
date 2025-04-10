/*
  ==============================================================================

    MuteButton.cpp
    Created: 27 Feb 2025 11:35:58am
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MuteButton.h"

//==============================================================================
MuteButton::MuteButton(juce::ValueTree& channelSettings) : juce::ToggleButton(), channelSettings(channelSettings)
{
    SignalManagerUI::getInstance().addListener(this);

    setToggleable(true);
    setToggleState(channelSettings.getProperty("Mute"), false);
    setClickingTogglesState(true);
    onClick = [this] { setMute(getToggleState()); };
}

MuteButton::~MuteButton()
{
    SignalManagerUI::getInstance().removeListener(this);
}

void MuteButton::handleMessage(const juce::Message& message)
{
    if (auto* signalMsg = dynamic_cast<const SignalMessage*>(&message)) {
        auto signal = static_cast<SignalManagerUI::Signal>(signalMsg->getSignalType());
        // Handle signal (already on message thread)...

        switch (signal)
        {
        case SignalManagerUI::Signal::RESTORE_UI_PARAMETERS:
            setToggleState(channelSettings.getProperty("Mute"), false);
            break;
        default:
            break;
        }
    }
}

void MuteButton::setMute(bool muteButtonToggleState)
{
    channelSettings.setProperty("Mute", muteButtonToggleState, nullptr);
}