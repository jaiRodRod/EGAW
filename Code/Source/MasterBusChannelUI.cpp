/*
  ==============================================================================

    MasterBusChannelUI.cpp
    Created: 24 Feb 2025 12:01:01pm
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MasterBusChannelUI.h"

//==============================================================================
MasterBusChannelUI::MasterBusChannelUI(juce::ValueTree& projectData) 
    : flexContainer(juce::FlexBox::Direction::column, juce::FlexBox::Wrap::noWrap
        , juce::FlexBox::AlignContent::flexStart, juce::FlexBox::AlignItems::flexStart
        , juce::FlexBox::JustifyContent::flexStart)
    , projectData(projectData)
    , routeIn(juce::Uuid("MasterBusChannel").toString())
    , panKnobSlider(projectData.getChildWithName(juce::Uuid("MasterBusChannel").toString()))
    , channelVolumeSlider(projectData.getChildWithName(juce::Uuid("MasterBusChannel").toString()))
    , muteSoloComp(projectData, juce::Uuid("MasterBusChannel").toString())
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    RoutingActionStateManager::getInstance().addListener(this);

    routeIn.setButtonText("Route In");
    routeIn.setEnabled(false);
    addAndMakeVisible(routeIn);
    routeIn.setAlwaysOnTop(true);

    addAndMakeVisible(panKnobSlider);

    addAndMakeVisible(channelVolumeSlider);

    addAndMakeVisible(muteSoloComp);

}

MasterBusChannelUI::~MasterBusChannelUI()
{
    RoutingActionStateManager::getInstance().removeListener(this);
}

void MasterBusChannelUI::handleMessage(const juce::Message& message)
{
    if (const auto* routingMsg = dynamic_cast<const RoutingMessage*>(&message))
    {
        const auto state = static_cast<RoutingActionStateManager::RoutingState>(routingMsg->routingState);

        switch (state)
        {
        case RoutingActionStateManager::RoutingState::ROUTING_OFF:
            routeIn.setEnabled(false);
            break;
        case RoutingActionStateManager::RoutingState::ROUTING_ON:
            if (!originAlreadyRoutedIn())
                routeIn.setEnabled(true);
            break;
        case RoutingActionStateManager::RoutingState::REMOVING_ROUTE:
            if (originAlreadyRoutedIn())
                routeIn.setEnabled(true);
            break;
        case RoutingActionStateManager::RoutingState::VIEWING_ROUTES:
            break;
        default:
            break;
        }
    }
}

void MasterBusChannelUI::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::blue);   // clear the background
}

void MasterBusChannelUI::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    auto area = getLocalBounds();

    flexContainer.items.clear();

    flexContainer.items.add(juce::FlexItem(routeIn).withHeight((float)area.getWidth()).withWidth(area.getWidth()));
    flexContainer.items.add(juce::FlexItem(panKnobSlider).withHeight((float)area.getWidth()).withWidth(area.getWidth()));
    flexContainer.items.add(juce::FlexItem(channelVolumeSlider).withHeight(4*(float)area.getWidth()).withWidth(area.getWidth()));
    flexContainer.items.add(juce::FlexItem(muteSoloComp).withHeight((float)area.getWidth()).withWidth(area.getWidth()));

    flexContainer.performLayout(area);
}

bool MasterBusChannelUI::originAlreadyRoutedIn()
{
    auto originChannelUuid = RoutingActionStateManager::getInstance().getOriginChannelUuid();
    auto channelData = projectData.getChildWithName(originChannelUuid);
    auto channelsRoutedToOriginData = channelData.getChildWithName("channelsRoutedTo");

    auto thisUuid = juce::Uuid("MasterBusChannel").toString();

    for (int i = 0; i < channelsRoutedToOriginData.getNumChildren(); ++i)
    {
        if (channelsRoutedToOriginData.getChild(i).getType().toString() == thisUuid)
        {
            return true;
        }
    }
    return false;
}
