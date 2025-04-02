/*
  ==============================================================================

    SoloButton.cpp
    Created: 4 Mar 2025 9:36:52am
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SoloButton.h"

//==============================================================================
SoloButton::SoloButton(juce::ValueTree& channelSettings) : juce::ToggleButton(), channelSettings(channelSettings)
{
    SignalManagerUI::getInstance().addListener(this);

    setToggleable(true);
    setToggleState(channelSettings.getProperty("Solo"), false);
    setClickingTogglesState(true);
    onClick = [this] { setSolo(getToggleState()); };

}

SoloButton::~SoloButton()
{
    SignalManagerUI::getInstance().removeListener(this);
}

void SoloButton::handleMessage(const juce::Message& message)
{
    if (auto* signalMsg = dynamic_cast<const SignalMessage*>(&message)) {
        auto signal = static_cast<SignalManagerUI::Signal>(signalMsg->getSignalType());
        // Handle signal (already on message thread)...

        switch (signal)
        {
        case SignalManagerUI::Signal::RESTORE_UI_PARAMETERS:
            setToggleState(channelSettings.getProperty("Solo"), false);
            break;
        default:
            break;
        }
    }
}

void SoloButton::setSolo(bool soloButtonToggleState)
{
    channelSettings.setProperty("Solo", soloButtonToggleState, nullptr);
    //DBG(channelSettings.toXmlString());
}
