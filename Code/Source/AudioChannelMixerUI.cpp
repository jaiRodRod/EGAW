/*
  ==============================================================================

    AudioChannelMixerUI.cpp
    Created: 24 Feb 2025 12:32:21pm
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AudioChannelMixerUI.h"

//==============================================================================
AudioChannelMixerUI::AudioChannelMixerUI(juce::ValueTree& projectData, juce::String channelUuid) 
    : flexContainer(juce::FlexBox::Direction::column, juce::FlexBox::Wrap::noWrap
        , juce::FlexBox::AlignContent::flexStart, juce::FlexBox::AlignItems::flexStart
        , juce::FlexBox::JustifyContent::flexStart)
    , projectData(projectData)
    , channelUuid(channelUuid)
    , channelColourSelector(projectData.getChildWithName(channelUuid))
    , channelNameLabel(projectData.getChildWithName(channelUuid))
    , deleteButton(projectData, channelUuid)
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

    addAndMakeVisible(panKnobSlider);

    addAndMakeVisible(channelVolumeSlider);

    addAndMakeVisible(muteSoloComp);

    routeTo.setButtonText("Route to");
    addAndMakeVisible(routeTo);

    addAndMakeVisible(moveComponent);
}

AudioChannelMixerUI::~AudioChannelMixerUI()
{
    RoutingActionStateManager::getInstance().removeListener(this);
}

void AudioChannelMixerUI::handleMessage(const juce::Message& message)
{
    if (const auto* routingMsg = dynamic_cast<const RoutingMessage*>(&message))
    {
        const auto state = static_cast<RoutingActionStateManager::RoutingState>(routingMsg->routingState);

        switch (state)
        {
        case RoutingActionStateManager::RoutingState::ROUTING_ON:
            routeTo.setEnabled(false);
            break;
        case RoutingActionStateManager::RoutingState::REMOVING_ROUTE:
            routeTo.setEnabled(false);
            break;
        case RoutingActionStateManager::RoutingState::ROUTING_OFF:
            routeTo.setEnabled(true);
            break;
        default:
            break;
        }
    }
}

void AudioChannelMixerUI::paint (juce::Graphics& g)
{
    auto colourID = projectData.getChildWithName(channelUuid).getProperty("Colour").toString();
    g.fillAll (juce::Colour::fromString(colourID));
}

void AudioChannelMixerUI::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    //setBounds(getLocalBounds());
    auto area = getLocalBounds();

    flexContainer.items.clear();

    flexContainer.items.add(juce::FlexItem(channelColourSelector).withHeight((float)area.getWidth() / 3).withWidth(area.getWidth()));
    flexContainer.items.add(juce::FlexItem(channelNameLabel).withHeight((float)area.getWidth() / 3).withWidth(area.getWidth()));
    flexContainer.items.add(juce::FlexItem(deleteButton).withHeight((float)area.getWidth() / 3).withWidth(area.getWidth()));
    flexContainer.items.add(juce::FlexItem(panKnobSlider).withHeight((float)area.getWidth()).withWidth(area.getWidth()));
    flexContainer.items.add(juce::FlexItem(channelVolumeSlider).withHeight(4 * (float)area.getWidth()).withWidth(area.getWidth()));
    flexContainer.items.add(juce::FlexItem(muteSoloComp).withHeight((float)area.getWidth() / 2).withWidth(area.getWidth()));
    flexContainer.items.add(juce::FlexItem(routeTo).withMinHeight((float)area.getWidth()).withMinWidth(area.getWidth()));
    flexContainer.items.add(juce::FlexItem(moveComponent).withHeight((float)area.getWidth() / 3).withWidth(area.getWidth()));

    flexContainer.performLayout(area);
}

juce::String AudioChannelMixerUI::getChannelUuid()
{
    return channelUuid;
}
