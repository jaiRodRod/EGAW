/*
  ==============================================================================

    PlaylistView.h
    Created: 13 Mar 2025 5:15:45pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "DisplaySingleton.h"
#include "SignalManagerUI.h"

#include "BidirectionalLinkedViewport.h"

#include "TransportBoxComponent.h"
#include "PlaylistChannelsView.h"
#include "PlaylistThumbnailChannelsView.h"
#include "CustomScrollBar_LF_01.h"

#include "ColourConstants.h"

//==============================================================================

class PlaylistView  : public juce::Component, public juce::ValueTree::Listener
{
public:
    PlaylistView(juce::ValueTree&, juce::ValueTree&, GlobalPlayhead&);
    ~PlaylistView() override;

    void valueTreeChildAdded (juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded) override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    int getNumAudioChannels();
    void setupGridLayout();

    juce::ValueTree projectData;
    juce::ValueTree playheadState;

	juce::Grid playlistGrid;

    juce::Component dummy_00;
	TransportBoxComponent transportBoxComponent;
    PlaylistChannelsView playlistChannelsView;
	PlaylistThumbnailChannelsView playlistThumbnailChannelsView;

	CustomScrollBar_LF customScrollBar_LF;

	BidirectionalLinkedViewport horizontalTransportViewport;
    BidirectionalLinkedViewport verticalPlaylistChannelsViewport;
    BidirectionalLinkedViewport bidirectionalPlaylistThumbnailChannelsViewport;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistView)
};
