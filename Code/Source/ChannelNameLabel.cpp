/*
  ==============================================================================

    ChannelNameLabel.cpp
    Created: 7 Mar 2025 9:45:06am
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ChannelNameLabel.h"

//==============================================================================
ChannelNameLabel::ChannelNameLabel(juce::ValueTree& channelSettings) : juce::Label(), channelSettings(channelSettings)
{
    SignalManagerUI::getInstance()->addListener(&valueListener);
    onTextChange = [this] { textChange(); };
    setEditable(false, true, false);
    setText(channelSettings.getProperty("Name"), juce::NotificationType::dontSendNotification);
}

ChannelNameLabel::~ChannelNameLabel()
{
    SignalManagerUI::getInstance()->removeListener(&valueListener);
}

void ChannelNameLabel::valueChanged(juce::Value& value)
{
    juce::Label::valueChanged(value);
    if (value == SignalManagerUI::getInstance()->getValue())
    {
        auto signal = SignalManagerUI::getInstance()->getCurrentSignal();

        switch (signal)
        {
        case SignalManagerUI::Signal::RESTORE_UI_PARAMETERS:
            DBG(channelSettings.toXmlString());
            setText(channelSettings.getProperty("Name"), juce::NotificationType::dontSendNotification);
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

/*
void ChannelNameLabel::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("ChannelNameLabel", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void ChannelNameLabel::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
*/