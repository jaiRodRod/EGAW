/*
  ==============================================================================

    MasterBusChannelUI.h
    Created: 24 Feb 2025 12:01:01pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RoutingActionStateManager.h"
#include "ChannelNameLabel.h"
#include "RouteInButton.h"
#include "MuteSoloComponent.h"
#include "ChannelVolumeSlider.h"
#include "PanKnobSlider.h"

#include "ColourConstants.h"

#include "MixerInterfaceFocusSingleton.h"

//==============================================================================
/*
*/
class MasterBusChannelUI  : public juce::Component, public juce::MessageListener
{
public:
    MasterBusChannelUI(juce::ValueTree&);
    ~MasterBusChannelUI() override;

	void handleMessage(const juce::Message& message) override;

    void paint (juce::Graphics&) override;
    void resized() override;

    juce::Point<int> getRouteInButtonCenter();

private:

    bool originAlreadyRoutedIn();

    juce::ValueTree projectData;

    //juce::Grid channelVerticalGrid;
    juce::FlexBox flexContainer;
	ChannelNameLabel channelNameLabel;
    RouteInButton routeIn;
    MuteSoloComponent muteSoloComp;
    ChannelVolumeSlider channelVolumeSlider;
    PanKnobSlider panKnobSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterBusChannelUI);
};
