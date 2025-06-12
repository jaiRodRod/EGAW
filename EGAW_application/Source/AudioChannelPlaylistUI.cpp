/*
  ==============================================================================

    AudioChannelPlaylistUI.cpp
    Created: 13 Mar 2025 5:16:46pm
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AudioChannelPlaylistUI.h"

//==============================================================================
AudioChannelPlaylistUI::AudioChannelPlaylistUI(juce::ValueTree& projectData, juce::String channelUuid) 
    : flexContainer(juce::FlexBox::Direction::row, juce::FlexBox::Wrap::noWrap
        , juce::FlexBox::AlignContent::flexStart, juce::FlexBox::AlignItems::flexStart
        , juce::FlexBox::JustifyContent::flexStart)
	, verticalFlexBox(juce::FlexBox::Direction::column, juce::FlexBox::Wrap::noWrap
		, juce::FlexBox::AlignContent::flexStart, juce::FlexBox::AlignItems::flexStart
		, juce::FlexBox::JustifyContent::flexStart)
    , projectData(projectData), channelUuid(channelUuid)
	, channelColourSelector(projectData.getChildWithName(channelUuid))
	, deleteButton(projectData.getChildWithName(channelUuid), channelUuid, true)
	, channelNameLabel(projectData.getChildWithName(channelUuid))
	, muteSoloComponentPlaylist(projectData, channelUuid)
{
	addAndMakeVisible(channelColourSelector);
	addAndMakeVisible(deleteButton);
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

	verticalFlexBox.items.clear();
	verticalFlexBox.items.add(juce::FlexItem(channelColourSelector).withHeight((float)area.getHeight()/2).withWidth(area.getWidth() * 0.25f));
	verticalFlexBox.items.add(juce::FlexItem(deleteButton).withHeight((float)area.getHeight()/2).withWidth(area.getWidth() * 0.25f));
	// auto verticalArea = area.constrainedWithin(juce::Rectangle<int>(0, 0, (int)(area.getWidth() * 0.25f), (int)(area.getHeight() / 2)));
	// verticalFlexBox.performLayout(verticalArea);

	flexContainer.items.add(juce::FlexItem(verticalFlexBox).withHeight((float)area.getHeight()).withWidth(area.getWidth() * 0.25f));
	flexContainer.items.add(juce::FlexItem(channelNameLabel).withHeight((float)area.getHeight()).withWidth(area.getWidth() * 0.5f));
	flexContainer.items.add(juce::FlexItem(muteSoloComponentPlaylist).withHeight((float)area.getHeight()).withWidth(area.getWidth() * 0.25f));

    flexContainer.performLayout(area);
}

juce::String AudioChannelPlaylistUI::getChannelUuid()
{
    return channelUuid;
}
