/*
  ==============================================================================

    MixBusChannelUI.h
    Created: 24 Feb 2025 11:39:16am
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RoutingActionStateManager.h"
#include "ChannelColourSelector.h"
#include "ChannelNameLabel.h"
#include "DeleteButton.h"
#include "RouteInButton.h"
#include "PanKnobSlider.h"
#include "ChannelVolumeSlider.h"
#include "MuteSoloComponent.h"
#include "RouteToButton.h"
#include "MoveComponent.h"

//==============================================================================
/*
*/
class MixBusChannelUI : public juce::Component, public juce::Value::Listener
{
public:
    MixBusChannelUI(juce::ValueTree&, juce::String);
    ~MixBusChannelUI() override;

    void valueChanged(juce::Value&) override;

    void paint (juce::Graphics&) override;
    void resized() override;

    juce::String getChannelUuid();

private:

    bool originAlreadyRoutedIn();

    juce::ValueTree projectData;
    juce::String channelUuid;

    juce::FlexBox flexContainer;
    ChannelColourSelector channelColourSelector;
    ChannelNameLabel channelNameLabel;
    DeleteButton deleteButton;
    RouteInButton routeIn;
    PanKnobSlider panKnobSlider;
    ChannelVolumeSlider channelVolumeSlider;
    MuteSoloComponent muteSoloComp;
    RouteToButton routeTo;
    MoveComponent moveComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MixBusChannelUI);
};
