/*
  ==============================================================================

    PluginRackChannelUI.h
    Created: 18 Apr 2025 11:35:40am
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
#include "PluginListContainerUI.h"
#include "MuteSoloComponent.h"
#include "RouteToButton.h"
#include "ViewRoutesChannel.h"
#include "MoveComponent.h"

//==============================================================================
/*
*/
class PluginRackChannelUI : public juce::Component, public juce::MessageListener
{
public:
    PluginRackChannelUI(juce::ValueTree&, juce::String);
    ~PluginRackChannelUI() override;

    void handleMessage(const juce::Message& message) override;

    void paint (juce::Graphics&) override;
    void resized() override;

    juce::String getChannelUuid();

	juce::Point<int> getRouteToButtonCenter();
	juce::Point<int> getRouteInButtonCenter();

private:

    bool alreadyRoutedIn();
	bool originRoutedIn();

    juce::ValueTree projectData;
    juce::String channelUuid;

    juce::FlexBox flexContainer;
    ChannelColourSelector channelColourSelector;
    ChannelNameLabel channelNameLabel;
    DeleteButton deleteButton;
    RouteInButton routeIn;
	PluginListContainerUI pluginListContainer;
    MuteSoloComponent muteSoloComp;
    RouteToButton routeTo;
	ViewRoutesChannel viewRoutesChannel;
    MoveComponent moveComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginRackChannelUI)
};
