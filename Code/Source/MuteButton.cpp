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
    SignalManagerUI::getInstance()->addListener(this);

    setToggleable(true);
    setToggleState(channelSettings.getProperty("Mute"), false);
    setClickingTogglesState(true);
    onClick = [this] { setMute(getToggleState()); };
}

MuteButton::~MuteButton()
{
    SignalManagerUI::getInstance()->removeListener(this);
}

void MuteButton::valueChanged(juce::Value& value)
{
    auto signal = SignalManagerUI::getInstance()->getCurrentSignal();

    switch (signal)
    {
        case SignalManagerUI::Signal::RESTORE_UI_PARAMETERS:
            setToggleState(channelSettings.getProperty("Mute"), false);
            break;
        default:
            break;
    }
}

void MuteButton::setMute(bool muteButtonToggleState)
{
    channelSettings.setProperty("Mute", muteButtonToggleState, nullptr);
}

/*
void MuteButton::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("MuteButton", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void MuteButton::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
*/