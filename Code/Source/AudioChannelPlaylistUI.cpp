/*
  ==============================================================================

    AudioChannelPlaylistUI.cpp
    Created: 13 Mar 2025 5:16:46pm
    Author:  admin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AudioChannelPlaylistUI.h"

//==============================================================================
AudioChannelPlaylistUI::AudioChannelPlaylistUI(juce::ValueTree& projectData, juce::String channelUuid) 
    : flexContainer(juce::FlexBox::Direction::row, juce::FlexBox::Wrap::noWrap
        , juce::FlexBox::AlignContent::flexStart, juce::FlexBox::AlignItems::flexStart
        , juce::FlexBox::JustifyContent::flexStart)
    , projectData(projectData), channelUuid(channelUuid)
	, channelColourSelector(projectData.getChildWithName(channelUuid))
	, channelNameLabel(projectData.getChildWithName(channelUuid))
	, muteSoloComponentPlaylist(projectData, channelUuid)
{
	addAndMakeVisible(channelColourSelector);
	addAndMakeVisible(channelNameLabel);
	addAndMakeVisible(muteSoloComponentPlaylist);
}

AudioChannelPlaylistUI::~AudioChannelPlaylistUI()
{
}

void AudioChannelPlaylistUI::paint (juce::Graphics& g)
{
    auto colourID = projectData.getChildWithName(channelUuid).getProperty("Colour").toString();
    g.fillAll (juce::Colour::fromString(colourID));
}

void AudioChannelPlaylistUI::resized()
{
    auto area = getLocalBounds();

    flexContainer.items.clear();

	flexContainer.items.add(juce::FlexItem(channelColourSelector).withHeight((float)area.getHeight()).withWidth(area.getWidth() * 0.25f));
	flexContainer.items.add(juce::FlexItem(channelNameLabel).withHeight((float)area.getHeight()).withWidth(area.getWidth() * 0.5f));
	flexContainer.items.add(juce::FlexItem(muteSoloComponentPlaylist).withHeight((float)area.getHeight()).withWidth(area.getWidth() * 0.25f));

    flexContainer.performLayout(area);
}

juce::String AudioChannelPlaylistUI::getChannelUuid()
{
    return channelUuid;
}
