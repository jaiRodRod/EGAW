/*
  ==============================================================================

    AudioChannelMixerUI.h
    Created: 24 Feb 2025 12:32:21pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RoutingActionStateManager.h"
#include "DisplaySingleton.h"
#include "ChannelColourSelector.h"
#include "ChannelNameLabel.h"
#include "DeleteButton.h"
#include "PanKnobSlider.h"
#include "ChannelVolumeSlider.h"
#include "MuteSoloComponent.h"
#include "RouteToButton.h"
#include "ViewRoutesChannel.h"
#include "MoveComponent.h"

#include "MixerInterfaceFocusSingleton.h"
#include "ChannelRouteViewSingleton.h"

//==============================================================================
/*
*/
class AudioChannelMixerUI  : public juce::Component, public juce::MessageListener
{
public:
    AudioChannelMixerUI(juce::ValueTree&, juce::String);
    ~AudioChannelMixerUI() override;

	void handleMessage(const juce::Message& message) override;

    void paint (juce::Graphics&) override;
    void resized() override;

    juce::String getChannelUuid();

	juce::Point<int> getRouteToButtonCenter();

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
	ViewRoutesChannel viewRoutesChannel;
    MoveComponent moveComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioChannelMixerUI);
};
