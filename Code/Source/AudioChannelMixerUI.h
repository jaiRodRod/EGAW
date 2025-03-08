/*
  ==============================================================================

    AudioChannelMixerUI.h
    Created: 24 Feb 2025 12:32:21pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SignalManagerUI.h"
#include "RoutingActionStateManager.h"
#include "DisplaySingleton.h"
#include "ChannelColourSelector.h"
#include "ChannelNameLabel.h"
#include "DeleteButton.h"
#include "PanKnobSlider.h"
#include "ChannelVolumeSlider.h"
#include "MuteSoloComponent.h"
#include "RouteToButton.h"
#include "MoveComponent.h"

//==============================================================================
/*
*/
class AudioChannelMixerUI  : public juce::Component, public juce::Value::Listener
{
public:
    AudioChannelMixerUI(juce::ValueTree&, juce::String);
    ~AudioChannelMixerUI() override;

    void valueChanged(juce::Value&) override;

    void paint (juce::Graphics&) override;
    void resized() override;

    juce::String getChannelUuid();

private:

    //void doRouteTo();
    //void doRemoveRouteTo();

    juce::ValueTree& projectData;
    juce::String channelUuid;

    juce::FlexBox flexContainer;
    
    ChannelColourSelector channelColourSelector;
    ChannelNameLabel channelNameLabel;
    DeleteButton deleteButton;
    PanKnobSlider panKnobSlider;
    ChannelVolumeSlider channelVolumeSlider;
    MuteSoloComponent muteSoloComp;
    RouteToButton routeTo;
    MoveComponent moveComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioChannelMixerUI);
};
