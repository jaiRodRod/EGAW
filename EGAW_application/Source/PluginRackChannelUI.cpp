/*
  ==============================================================================

    PluginRackChannelUI.cpp
    Created: 18 Apr 2025 11:35:40am
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PluginRackChannelUI.h"

//==============================================================================
PluginRackChannelUI::PluginRackChannelUI(juce::ValueTree& projectData, juce::String channelUuid)
    : flexContainer(juce::FlexBox::Direction::column, juce::FlexBox::Wrap::noWrap
        , juce::FlexBox::AlignContent::flexStart, juce::FlexBox::AlignItems::flexStart
        , juce::FlexBox::JustifyContent::flexStart)
    , projectData(projectData)
    , channelUuid(channelUuid)
    , channelNameLabel(projectData.getChildWithName(channelUuid))
    , channelColourSelector(projectData.getChildWithName(channelUuid))
    , deleteButton(projectData, channelUuid, false)
    , routeIn(channelUuid)
	, pluginListContainer(projectData, projectData.getChildWithName(channelUuid).getChildWithName("plugins"), channelUuid)
    , muteSoloComp(projectData, channelUuid)
    , routeTo(channelUuid)
	, viewRoutesChannel(channelUuid)
    , moveComponent(projectData, channelUuid)
{
    RoutingActionStateManager::getInstance().addListener(this);

    addAndMakeVisible(channelColourSelector);

    addAndMakeVisible(channelNameLabel);

    addAndMakeVisible(deleteButton);

    routeIn.setButtonText("Route In");
    routeIn.setEnabled(false);
    addAndMakeVisible(routeIn);
    routeIn.setAlwaysOnTop(true);

	addAndMakeVisible(pluginListContainer);

    addAndMakeVisible(muteSoloComp);

    routeTo.setButtonText("Route to");
    addAndMakeVisible(routeTo);

	addAndMakeVisible(viewRoutesChannel);

    addAndMakeVisible(moveComponent);
}

PluginRackChannelUI::~PluginRackChannelUI()
{
    RoutingActionStateManager::getInstance().removeListener(this);
}

void PluginRackChannelUI::handleMessage(const juce::Message& message)
{
    if (const auto* routingMsg = dynamic_cast<const RoutingMessage*>(&message))
    {
        const auto state = static_cast<RoutingActionStateManager::RoutingState>(routingMsg->routingState);

        switch (state)
        {
        case RoutingActionStateManager::RoutingState::ROUTING_ON:
            if (!alreadyRoutedIn())
                routeIn.setEnabled(true);
            routeTo.setEnabled(false);
            break;
        case RoutingActionStateManager::RoutingState::REMOVING_ROUTE:
            if (originRoutedIn())
                routeIn.setEnabled(true);
            routeTo.setEnabled(false);
            break;
        case RoutingActionStateManager::RoutingState::ROUTING_OFF:
            routeIn.setEnabled(false);
            routeTo.setEnabled(true);
            SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
            break;
        default:
            break;
        }
    }
}

void PluginRackChannelUI::paint (juce::Graphics& g)
{
    auto colourID = projectData.getChildWithName(channelUuid).getProperty("Colour").toString();
    g.fillAll (juce::Colour::fromString(colourID));
}

void PluginRackChannelUI::resized()
{
    auto area = getLocalBounds();

    if (!routeIn.isEnabled())
    {
        routeIn.setAlpha(0.5f);
    }
    else
    {
        routeIn.setAlpha(1.0f);
    }

    flexContainer.items.clear();

    flexContainer.items.add(juce::FlexItem(channelColourSelector).withHeight(((float)area.getWidth() / 2) / 3).withWidth(area.getWidth()));
    flexContainer.items.add(juce::FlexItem(channelNameLabel).withHeight(((float)area.getWidth() / 2) / 3).withWidth(area.getWidth()));
    flexContainer.items.add(juce::FlexItem(deleteButton).withHeight(((float)area.getWidth() / 2) / 3).withWidth(area.getWidth()));
    flexContainer.items.add(juce::FlexItem(routeIn).withHeight(((float)area.getWidth() / 4)).withWidth(area.getWidth()));
	flexContainer.items.add(juce::FlexItem(pluginListContainer).withHeight(((float)area.getWidth() / 2) * 5).withWidth(area.getWidth()));
    flexContainer.items.add(juce::FlexItem(muteSoloComp).withHeight(((float)area.getWidth() / 2) / 2).withWidth(area.getWidth()));
    flexContainer.items.add(juce::FlexItem(routeTo).withHeight(((float)area.getWidth() / 4)).withWidth(area.getWidth()));
	flexContainer.items.add(juce::FlexItem(viewRoutesChannel).withHeight(((float)area.getWidth() / 2) / 3).withWidth(area.getWidth()));
    flexContainer.items.add(juce::FlexItem(moveComponent).withHeight(((float)area.getWidth() / 2) / 3).withWidth(area.getWidth()));

    flexContainer.performLayout(area);
}

juce::String PluginRackChannelUI::getChannelUuid()
{
    return channelUuid;
}

juce::Point<int> PluginRackChannelUI::getRouteToButtonCenter()
{
    auto screenPoint = routeTo.localPointToGlobal(routeTo.getComponentCenter());

    return getLocalPoint(nullptr, screenPoint);
}

juce::Point<int> PluginRackChannelUI::getRouteInButtonCenter()
{
    auto screenPoint = routeIn.localPointToGlobal(routeIn.getComponentCenter());

    return getLocalPoint(nullptr, screenPoint);
}

bool PluginRackChannelUI::alreadyRoutedIn()
{
    auto thisUuid = getChannelUuid();
    auto originChannelUuid = RoutingActionStateManager::getInstance().getOriginChannelUuid();

    if (thisUuid == originChannelUuid)
        return true;

	auto channelOrder = projectData.getChildWithName("channelOrder");

    for (int i = 0; i < channelOrder.getNumChildren(); ++i)
    {
		auto channelData = projectData.getChildWithName(channelOrder.getChild(i).getType().toString());
        auto channelsRoutedTo = channelData.getChildWithName("channelsRoutedTo");
        for (int i = 0; i < channelsRoutedTo.getNumChildren(); ++i)
        {
            if (channelsRoutedTo.getChild(i).getType().toString() == thisUuid)
            {
                return true;
            }
        }
    }    
    return false;
}

bool PluginRackChannelUI::originRoutedIn()
{
    auto originChannelUuid = RoutingActionStateManager::getInstance().getOriginChannelUuid();
    auto thisUuid = getChannelUuid();

    auto channelData = projectData.getChildWithName(originChannelUuid);
    auto channelsRoutedToOriginData = channelData.getChildWithName("channelsRoutedTo");


    for (int i = 0; i < channelsRoutedToOriginData.getNumChildren(); ++i)
    {
        if (channelsRoutedToOriginData.getChild(i).getType().toString() == thisUuid)
        {
            return true;
        }
    }
    return false;
}
