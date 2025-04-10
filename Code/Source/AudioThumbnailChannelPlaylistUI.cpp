/*
  ==============================================================================

    AudioThumbnailChannelPlaylistUI.cpp
    Created: 15 Mar 2025 5:46:24pm
    Author:  admin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AudioThumbnailChannelPlaylistUI.h"

//==============================================================================
AudioThumbnailChannelPlaylistUI::AudioThumbnailChannelPlaylistUI(juce::ValueTree& projectData, juce::ValueTree& playheadState, juce::String channelUuid, GlobalPlayhead& globalPlayhead) 
    : projectData(projectData)
	, playheadState(playheadState)
    , channelUuid(channelUuid)
	, globalPlayhead(globalPlayhead)
    , formatManager() // Initialize the formatManager
    , thumbnailCache(5) // Cache size
    , thumbnail((int)playheadState.getProperty("bufferSize"), formatManager, thumbnailCache) // Pass formatManager to the thumbnail
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    // Register basic audio formats (WAV, AIFF, etc.)
    formatManager.registerBasicFormats();

    // Load the audio file from the project data
    audioFile = juce::File(projectData.getChildWithName(channelUuid).getProperty("FilePath").toString());
    thumbnail.setSource(new juce::FileInputSource(audioFile));
}

AudioThumbnailChannelPlaylistUI::~AudioThumbnailChannelPlaylistUI()
{

}

void AudioThumbnailChannelPlaylistUI::paint (juce::Graphics& g)
{
    auto colourID = projectData.getChildWithName(channelUuid).getProperty("Colour").toString();
    g.fillAll (juce::Colour::fromString(colourID));

    // Draw the audio thumbnail
    if (showAudioWave && thumbnail.getNumChannels() > 0)
    {
        g.setColour(juce::Colours::white);
		double audioFileStart = projectData.getChildWithName(channelUuid).getProperty("AudioFileStart", 0.0);
		double audioFileEnd = projectData.getChildWithName(channelUuid).getProperty("AudioFileEnd", 0.0);

        thumbnail.drawChannels(g, getLocalBounds(), audioFileStart, audioFileEnd, 1.0f); //CHECK
    }
    else
    {
        g.setColour(juce::Colours::red.withAlpha(0.5f));
        g.drawRect(getLocalBounds(), 2);
    }
}

void AudioThumbnailChannelPlaylistUI::resized()
{
    auto area = getLocalBounds();
}

void AudioThumbnailChannelPlaylistUI::mouseDown(const juce::MouseEvent& event)
{
    if (event.mods.isRightButtonDown())
    {
        juce::PopupMenu menu;

        menu.addItem("Show Audio Wave",
            true,  // can be ticked
            showAudioWave,  // current tick state
            [this] { toggleShowAudioWave(); });

		menu.addItem("Adjust Audio Time",
			true,  // can be ticked
			false,  // current tick state
			[this] {
				audioTimeAdjusterCall();
			});

        menu.showMenuAsync(juce::PopupMenu::Options()
            .withTargetComponent(this)
            .withMousePosition());
    }
    else
    {
        originalStartTime = (double)projectData.getChildWithName(channelUuid).getProperty("StartTime", 0.0);
    }
}

void AudioThumbnailChannelPlaylistUI::mouseDrag(const juce::MouseEvent& event)
{
    auto sampleRate = (double)playheadState.getProperty("sampleRate");

    auto dragStartPoint = event.getMouseDownPosition().getX();
    auto dragStartPosInSeconds = (double)dragStartPoint / (double)projectData.getProperty("Zoom");

    auto draggedDistanceX = event.getDistanceFromDragStartX();
    auto distanceInSeconds = (double)draggedDistanceX / (double)projectData.getProperty("Zoom");
    
	auto newStartTime = originalStartTime + distanceInSeconds;
    if (newStartTime < 0)
    {
		newStartTime = 0;
	}

    projectData.getChildWithName(channelUuid).setProperty("StartTime", newStartTime, nullptr);

	repaint();
}

juce::String AudioThumbnailChannelPlaylistUI::getChannelUuid()
{
    return channelUuid;
}

double AudioThumbnailChannelPlaylistUI::getThumbnailTotalLength()
{
    return thumbnail.getTotalLength();
}

void AudioThumbnailChannelPlaylistUI::toggleShowAudioWave()
{
	showAudioWave = !showAudioWave;
	repaint();
}

void AudioThumbnailChannelPlaylistUI::audioTimeAdjusterCall()
{
    auto audioFileStart = projectData.getChildWithName(channelUuid).getProperty("AudioFileStart", 0.0);
    auto audioFileEnd = projectData.getChildWithName(channelUuid).getProperty("AudioFileEnd", 0.0);
    auto audioFileLength = projectData.getChildWithName(channelUuid).getProperty("AudioFileLength", 0.0);

    showAudioTimeAdjusterPopup(audioFileStart, audioFileEnd, audioFileLength,
        [this](double newStart, double newEnd, bool accepted) {
            if (accepted)
            {
                auto audioFileLength = (double)projectData.getChildWithName(channelUuid).getProperty("AudioFileLength", 0.0);
				if (newStart < 0)
				{
					newStart = 0;
				}
                if (newEnd > audioFileLength)
                {
					newEnd = audioFileLength;
                }
                projectData.getChildWithName(channelUuid).setProperty("AudioFileStart", newStart, nullptr);
                projectData.getChildWithName(channelUuid).setProperty("AudioFileEnd", newEnd, nullptr);

                if (auto* parent = getParentComponent())
                {
                    parent->resized();
                    repaint();
                }
            }
        });
}
