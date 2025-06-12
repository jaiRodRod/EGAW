/*
  ==============================================================================

    PlaylistView.cpp
    Created: 13 Mar 2025 5:15:45pm
    Author:  jaezro

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
	, playlistThumbnailChannelsView(projectData, playheadState, globalPlayhead, customScrollBar_LF) // Pasar globalPlayhead para el contest for length
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
    bidirectionalPlaylistThumbnailChannelsViewport.getVerticalScrollBar().setLookAndFeel(&customScrollBar_LF);
    bidirectionalPlaylistThumbnailChannelsViewport.getHorizontalScrollBar().setLookAndFeel(&customScrollBar_LF);
    bidirectionalPlaylistThumbnailChannelsViewport.setScrollBarThickness(customScrollBar_LF.getDefaultScrollbarWidth()); // Width, Height
    bidirectionalPlaylistThumbnailChannelsViewport.setViewedComponent(&playlistThumbnailChannelsView, false);

    // Force overlay behavior
    bidirectionalPlaylistThumbnailChannelsViewport.getVerticalScrollBar().setAlwaysOnTop(true);
    bidirectionalPlaylistThumbnailChannelsViewport.getHorizontalScrollBar().setAlwaysOnTop(true);

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
    g.fillAll (colourConstants::dark_slate_blue_pressed);   // clear the background
	g.setColour(colourConstants::plain_green);
    g.fillRect(bidirectionalPlaylistThumbnailChannelsViewport.getBoundsInParent());
    g.setColour(juce::Colours::lightgrey.darker(0.1f));
    g.fillRect(bidirectionalPlaylistThumbnailChannelsViewport.getBoundsInParent().getRight() - customScrollBar_LF.getDefaultScrollbarWidth(),
        bidirectionalPlaylistThumbnailChannelsViewport.getBoundsInParent().getBottom() - customScrollBar_LF.getDefaultScrollbarWidth(),
        customScrollBar_LF.getDefaultScrollbarWidth(), customScrollBar_LF.getDefaultScrollbarWidth());
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

    // Force overlay behavior
    bidirectionalPlaylistThumbnailChannelsViewport.getVerticalScrollBar().setAlwaysOnTop(true);
    bidirectionalPlaylistThumbnailChannelsViewport.getHorizontalScrollBar().setAlwaysOnTop(true);
    bidirectionalPlaylistThumbnailChannelsViewport.getVerticalScrollBar().setSize(customScrollBar_LF.getDefaultScrollbarWidth(), bidirectionalPlaylistThumbnailChannelsViewport.getHeight()); // Width, Height
    bidirectionalPlaylistThumbnailChannelsViewport.getHorizontalScrollBar().setSize(bidirectionalPlaylistThumbnailChannelsViewport.getWidth(), customScrollBar_LF.getDefaultScrollbarWidth());

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
