/*
  ==============================================================================

    AudioThumbnailChannelPlaylistUI.h
    Created: 15 Mar 2025 5:46:24pm
    Author:  admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "GlobalPlayhead.h"

//==============================================================================
/*
*/
class AudioThumbnailChannelPlaylistUI  : public juce::Component
{
public:
    AudioThumbnailChannelPlaylistUI(juce::ValueTree&, juce::String);
    ~AudioThumbnailChannelPlaylistUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    juce::String getChannelUuid();
    double getThumbnailTotalLength();

private:

    juce::ValueTree projectData;
    juce::String channelUuid;

    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbnailCache;
    juce::AudioThumbnail thumbnail;
    juce::File audioFile;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioThumbnailChannelPlaylistUI)
};
