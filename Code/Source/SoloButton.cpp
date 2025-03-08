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
    SignalManagerUI::getInstance()->addListener(this);

    setToggleable(true);
    setToggleState(channelSettings.getProperty("Solo"), false);
    setClickingTogglesState(true);
    onClick = [this] { setSolo(getToggleState()); };

}

SoloButton::~SoloButton()
{
    SignalManagerUI::getInstance()->removeListener(this);
}

void SoloButton::valueChanged(juce::Value& value)
{
    auto signal = SignalManagerUI::getInstance()->getCurrentSignal();

    switch (signal)
    {
    case SignalManagerUI::Signal::RESTORE_UI_PARAMETERS:
        setToggleState(channelSettings.getProperty("Solo"), false);
        break;
    default:
        break;
    }
}

void SoloButton::setSolo(bool soloButtonToggleState)
{
    channelSettings.setProperty("Solo", soloButtonToggleState, nullptr);
    //DBG(channelSettings.toXmlString());
}

/*
void SoloButton::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("SoloButton", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void SoloButton::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
*/
