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
#include "AudioTimeAdjusterWindow.h"

//==============================================================================

class AudioThumbnailChannelPlaylistUI  : public juce::Component
{
public:
    AudioThumbnailChannelPlaylistUI(juce::ValueTree&, juce::ValueTree&, juce::String, GlobalPlayhead&);
    ~AudioThumbnailChannelPlaylistUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;
	void mouseDown (const juce::MouseEvent& event) override;
	void mouseDrag (const juce::MouseEvent& event) override;

    juce::String getChannelUuid();
    double getThumbnailTotalLength();

private:

	void toggleShowAudioWave();
    void audioTimeAdjusterCall();

    juce::ValueTree projectData;
	juce::ValueTree playheadState;
    juce::String channelUuid;
	GlobalPlayhead& globalPlayhead;

    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbnailCache;
    juce::AudioThumbnail thumbnail;
    juce::File audioFile;

	double originalStartTime = 0.0;
	bool showAudioWave = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioThumbnailChannelPlaylistUI)
};
