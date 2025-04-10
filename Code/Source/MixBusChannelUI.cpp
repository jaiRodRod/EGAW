/*
  ==============================================================================

    MixBusChannelUI.cpp
    Created: 24 Feb 2025 11:39:16am
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MixBusChannelUI.h"

//==============================================================================
MixBusChannelUI::MixBusChannelUI(juce::ValueTree& projectData, juce::String channelUuid) 
    : flexContainer(juce::FlexBox::Direction::column, juce::FlexBox::Wrap::noWrap
        , juce::FlexBox::AlignContent::flexStart, juce::FlexBox::AlignItems::flexStart
        , juce::FlexBox::JustifyContent::flexStart)
    , projectData(projectData)
    , channelUuid(channelUuid)
    , channelNameLabel(projectData.getChildWithName(channelUuid))
    , channelColourSelector(projectData.getChildWithName(channelUuid))
    , deleteButton(projectData, channelUuid)
    , routeIn(channelUuid)
    , panKnobSlider(projectData.getChildWithName(channelUuid))
    , channelVolumeSlider(projectData.getChildWithName(channelUuid))
    , muteSoloComp(projectData, channelUuid)
    , routeTo(channelUuid)
    , moveComponent(projectData, channelUuid)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    RoutingActionStateManager::getInstance().addListener(this);

    addAndMakeVisible(channelColourSelector);

    addAndMakeVisible(channelNameLabel);

    addAndMakeVisible(deleteButton);

    routeIn.setButtonText("Route In");
    routeIn.setEnabled(false);
    addAndMakeVisible(routeIn);
    routeIn.setAlwaysOnTop(true);

    addAndMakeVisible(panKnobSlider);

    addAndMakeVisible(channelVolumeSlider);

    addAndMakeVisible(muteSoloComp);

    routeTo.setButtonText("Route to");
    addAndMakeVisible(routeTo);

    addAndMakeVisible(moveComponent);
}

MixBusChannelUI::~MixBusChannelUI()
{
    RoutingActionStateManager::getInstance().removeListener(this);
}

void MixBusChannelUI::handleMessage(const juce::Message& message)
{
    if (const auto* routingMsg = dynamic_cast<const RoutingMessage*>(&message))
    {
        const auto state = static_cast<RoutingActionStateManager::RoutingState>(routingMsg->routingState);

        switch (state)
        {
        case RoutingActionStateManager::RoutingState::ROUTING_ON:
            if (!originAlreadyRoutedIn())
                routeIn.setEnabled(true);
            routeTo.setEnabled(false);
            break;
        case RoutingActionStateManager::RoutingState::REMOVING_ROUTE:
            if (originAlreadyRoutedIn())
                routeIn.setEnabled(true);
            routeTo.setEnabled(false);
            break;
        case RoutingActionStateManager::RoutingState::ROUTING_OFF:
            routeIn.setEnabled(false);
            routeTo.setEnabled(true);
            break;
        default:
            break;
        }
    }
}

void MixBusChannelUI::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get  started.
you
       You should replace everything in this method with your own
       drawing code..
    */

    auto colourID = projectData.getChildWithName(channelUuid).getProperty("Colour").toString();
    g.fillAll (juce::Colour::fromString(colourID));   // clear the background
}

void MixBusChannelUI::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    auto area = getLocalBounds();

    flexContainer.items.clear();

    flexContainer.items.add(juce::FlexItem(channelColourSelector).withHeight((float)area.getWidth() / 3).withWidth(area.getWidth()));
    flexContainer.items.add(juce::FlexItem(channelNameLabel).withHeight((float)area.getWidth() / 3).withWidth(area.getWidth()));
    flexContainer.items.add(juce::FlexItem(deleteButton).withHeight((float)area.getWidth() / 3).withWidth(area.getWidth()));
    flexContainer.items.add(juce::FlexItem(routeIn).withHeight((float)area.getWidth()).withWidth(area.getWidth()));
    flexContainer.items.add(juce::FlexItem(panKnobSlider).withHeight((float)area.getWidth()).withWidth(area.getWidth()));
    flexContainer.items.add(juce::FlexItem(channelVolumeSlider).withHeight(4 * (float)area.getWidth()).withWidth(area.getWidth()));
    flexContainer.items.add(juce::FlexItem(muteSoloComp).withHeight((float)area.getWidth() / 2).withWidth(area.getWidth()));
    flexContainer.items.add(juce::FlexItem(routeTo).withHeight((float)area.getWidth()).withWidth(area.getWidth()));
    flexContainer.items.add(juce::FlexItem(moveComponent).withHeight((float)area.getWidth() / 3).withWidth(area.getWidth()));

    flexContainer.performLayout(area);
}

juce::String MixBusChannelUI::getChannelUuid()
{
    return channelUuid;
}

bool MixBusChannelUI::originAlreadyRoutedIn()
{

    auto originChannelUuid = RoutingActionStateManager::getInstance().getOriginChannelUuid();
    auto thisUuid = getChannelUuid();

    if (thisUuid == originChannelUuid)
        return true;

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
