/*
  ==============================================================================

    MasterBusChannelUI.h
    Created: 24 Feb 2025 12:01:01pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SignalManagerUI.h"
#include "RoutingActionStateManager.h"
#include "RouteInButton.h"
#include "MuteSoloComponent.h"
#include "ChannelVolumeSlider.h"
#include "PanKnobSlider.h"

//==============================================================================
/*
*/
class MasterBusChannelUI  : public juce::Component, public juce::Value::Listener
{
public:
    MasterBusChannelUI(juce::ValueTree&);
    ~MasterBusChannelUI() override;

    void valueChanged(juce::Value&) override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    bool originAlreadyRoutedIn();

    juce::ValueTree projectData;

    //juce::Grid channelVerticalGrid;
    juce::FlexBox flexContainer;
    RouteInButton routeIn;
    MuteSoloComponent muteSoloComp;
    ChannelVolumeSlider channelVolumeSlider;
    PanKnobSlider panKnobSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterBusChannelUI);
};
