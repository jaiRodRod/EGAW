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
#include "ViewRoutesChannel.h"
#include "MoveComponent.h"

#include "MixerInterfaceFocusSingleton.h"

//==============================================================================
/*
*/
class MixBusChannelUI : public juce::Component, public juce::MessageListener
{
public:
    MixBusChannelUI(juce::ValueTree&, juce::String);
    ~MixBusChannelUI() override;

	void handleMessage(const juce::Message& message) override;

    void paint (juce::Graphics&) override;
    void resized() override;

    juce::String getChannelUuid();

    juce::Point<int> getRouteToButtonCenter();
    juce::Point<int> getRouteInButtonCenter();

private:

    bool originAlreadyRoutedIn(bool);

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
	ViewRoutesChannel viewRoutesChannel;
    MoveComponent moveComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MixBusChannelUI);
};
