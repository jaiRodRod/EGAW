/*
  ==============================================================================

    ChannelVolumeSlider.h
    Created: 4 Mar 2025 3:56:44pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SignalManagerUI.h"

//==============================================================================
/*
*/
class ChannelVolumeSlider  : public juce::Slider, public juce::MessageListener
{
public:
    ChannelVolumeSlider(juce::ValueTree&);
    ~ChannelVolumeSlider() override;

    //void paint (juce::Graphics&) override;
    //void resized() override;

	void handleMessage(const juce::Message& message) override;

    //Metodo para mostrar datos en las parametrizaciones
    void setDataMode(bool shouldBeOn);

private:

    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseDoubleClick(const juce::MouseEvent& event) override;
    void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel) override;
    void mouseDown(const juce::MouseEvent& event) override;

    juce::ValueTree channelSettings;
    bool isTextActive;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChannelVolumeSlider)
};
