/*
  ==============================================================================

    AudioChannelPlaylistUI.h
    Created: 13 Mar 2025 5:16:46pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "ChannelColourSelector.h"
#include "ChannelNameLabel.h"
#include "MuteSoloComponentPlaylist.h"
#include "DeleteButton.h"

//==============================================================================
/*
*/
class AudioChannelPlaylistUI  : public juce::Component
{
public:
    AudioChannelPlaylistUI(juce::ValueTree&, juce::String);
    ~AudioChannelPlaylistUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    juce::String getChannelUuid();

private:

    juce::ValueTree projectData;
    juce::String channelUuid;

    juce::FlexBox flexContainer;
	juce::FlexBox verticalFlexBox;
	ChannelColourSelector channelColourSelector;
	DeleteButton deleteButton;
    ChannelNameLabel channelNameLabel;
    MuteSoloComponentPlaylist muteSoloComponentPlaylist;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioChannelPlaylistUI)
};
