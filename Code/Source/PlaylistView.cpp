/*
  ==============================================================================

    PlaylistView.cpp
    Created: 13 Mar 2025 5:15:45pm
    Author:  admin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistView.h"

//==============================================================================
PlaylistView::PlaylistView(juce::ValueTree& projectData, juce::ValueTree& playheadState, GlobalPlayhead& globalPlayhead) 
    : projectData(projectData)
    , playheadState(playheadState)
	, playlistGrid()
	, transportBoxComponent(projectData, playheadState, globalPlayhead)
    , playlistChannelsView(projectData, playheadState)
	, playlistThumbnailChannelsView(projectData, playheadState, globalPlayhead) // Pasar globalPlayhead para el contest for length
{
    setupGridLayout();
    projectData.addListener(this);

	addAndMakeVisible(dummy_00);
    addAndMakeVisible(horizontalTransportViewport);
    addAndMakeVisible(verticalPlaylistChannelsViewport);
    addAndMakeVisible(bidirectionalPlaylistThumbnailChannelsViewport);

    horizontalTransportViewport.setScrollBarPosition(false, false);
    horizontalTransportViewport.setScrollBarsShown(false, false, false, true);
    horizontalTransportViewport.setViewedComponent(&transportBoxComponent, false);

    verticalPlaylistChannelsViewport.setScrollBarPosition(false, false);
    verticalPlaylistChannelsViewport.setScrollBarsShown(false, false, true, false);
    verticalPlaylistChannelsViewport.setViewedComponent(&playlistChannelsView, false);

    bidirectionalPlaylistThumbnailChannelsViewport.setScrollBarPosition(true, true);
    bidirectionalPlaylistThumbnailChannelsViewport.setScrollBarsShown(true, true, true, true);
    bidirectionalPlaylistThumbnailChannelsViewport.setViewedComponent(&playlistThumbnailChannelsView, false);

	horizontalTransportViewport.setHorizontalLinkedViewport(&bidirectionalPlaylistThumbnailChannelsViewport);
	verticalPlaylistChannelsViewport.setVerticalLinkedViewport(&bidirectionalPlaylistThumbnailChannelsViewport);
	bidirectionalPlaylistThumbnailChannelsViewport.setLinkedViewports(&verticalPlaylistChannelsViewport, &horizontalTransportViewport);
}

PlaylistView::~PlaylistView()
{
    projectData.removeListener(this);
}

void PlaylistView::valueTreeChildAdded(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded)
{
    if (parentTree == projectData)
    {
        juce::MessageManager::callAsync([this] {
            SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
        });
    }
}

void PlaylistView::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::purple);   // clear the background
}

void PlaylistView::resized()
{
    auto area = getLocalBounds();

    auto thumbnailLengthWidth = (double)projectData.getProperty("Zoom") * (double)playheadState.getProperty("time");


	dummy_00.setSize(area.getWidth() * 0.2f, area.getHeight() * 0.05f);

	transportBoxComponent.setSize(thumbnailLengthWidth, area.getHeight() * 0.05f);

    if (auto* display = DisplaySingleton::getInstance())
    {
		const float channelHeight = display->getMainDisplayBounds().getHeight() / 12;

        const float totalHeight = channelHeight * (getNumAudioChannels() + 1) +
            display->getMainDisplayBounds().getHeight() / 36;

        playlistChannelsView.setSize(area.getWidth() * 0.2f, totalHeight);

        playlistThumbnailChannelsView.setSize(thumbnailLengthWidth, totalHeight);
    }

	horizontalTransportViewport.setSize(area.getWidth() * 0.8f, area.getHeight() * 0.05f);
    verticalPlaylistChannelsViewport.setSize(area.getWidth() * 0.2f, area.getHeight() * 0.95f);
	bidirectionalPlaylistThumbnailChannelsViewport.setSize(area.getWidth() * 0.8f, area.getHeight() * 0.95f);
    playlistGrid.performLayout(area);
}

int PlaylistView::getNumAudioChannels()
{
    int total = 0;
    for (int i = 1; i < projectData.getNumChildren(); ++i)
    {
        if (projectData.getChild(i).getProperty("Type") == "AudioChannel")
        {
            ++total;
        }
    }
    return total;
}

void PlaylistView::setupGridLayout()
{
    playlistGrid.templateColumns = {
        juce::Grid::Fr(2),
        juce::Grid::Fr(8)
    };

    playlistGrid.templateRows = {
        juce::Grid::Fr(5),
        juce::Grid::Fr(95)
    };

    playlistGrid.items = {
        juce::GridItem(dummy_00).withArea(1,1), juce::GridItem(horizontalTransportViewport).withArea(1,2),
		juce::GridItem(verticalPlaylistChannelsViewport).withArea(2,1), juce::GridItem(bidirectionalPlaylistThumbnailChannelsViewport).withArea(2,2)
    };
}
