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
AudioThumbnailChannelPlaylistUI::AudioThumbnailChannelPlaylistUI(juce::ValueTree& projectData, juce::String channelUuid) 
    : projectData(projectData)
    , channelUuid(channelUuid)
    , formatManager() // Initialize the formatManager
    , thumbnailCache(5) // Cache size
    , thumbnail(512, formatManager, thumbnailCache) // Pass formatManager to the thumbnail
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
    if (thumbnail.getNumChannels() > 0)
    {
        g.setColour(juce::Colours::white);
        thumbnail.drawChannels(g, getLocalBounds(), 0.0, thumbnail.getTotalLength(), 1.0f);
    }
}

void AudioThumbnailChannelPlaylistUI::resized()
{
    auto area = getLocalBounds();
}

juce::String AudioThumbnailChannelPlaylistUI::getChannelUuid()
{
    return channelUuid;
}

double AudioThumbnailChannelPlaylistUI::getThumbnailTotalLength()
{
    return thumbnail.getTotalLength();
}
