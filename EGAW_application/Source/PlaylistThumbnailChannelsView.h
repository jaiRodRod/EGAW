/*
  ==============================================================================

    PlaylistThumbnailChannelsView.h
    Created: 13 Mar 2025 5:19:37pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ZoomConstants.h"

#include "SignalManagerUI.h"
#include "DisplaySingleton.h"
#include "AudioThumbnailChannelPlaylistUI.h"
#include "GlobalPlayhead.h"
#include "TransportBoxComponent.h"
#include "CustomScrollBar_LF_01.h"

#include "ZoomConstants.h"
#include "ColourConstants.h"

//==============================================================================
/*
*/
class PlaylistThumbnailChannelsView : public juce::Component, public juce::ValueTree::Listener, public juce::MessageListener, public juce::MouseListener
{
public:
    PlaylistThumbnailChannelsView(juce::ValueTree&, juce::ValueTree&, GlobalPlayhead&, CustomScrollBar_LF&);
    ~PlaylistThumbnailChannelsView() override;

    void valueTreePropertyChanged (juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override;
    void valueTreeChildAdded (juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded) override;
    void valueTreeChildRemoved (juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override;
    void valueTreeChildOrderChanged (juce::ValueTree& parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex) override;
	
    void handleMessage(const juce::Message& message) override;

	void mouseMagnify (const juce::MouseEvent& event, float scaleFactor) override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    void rebuildUI();
    void setupFlexBoxLayout();

    AudioThumbnailChannelPlaylistUI* getAudioThumbnailChannelPlaylistUI(juce::String);
    float calculateThumbnailWidth(double audioThumbnailChannelLength) const;

    juce::ValueTree projectData;
    juce::ValueTree playheadState;
	GlobalPlayhead& globalPlayhead;
	CustomScrollBar_LF& customScrollBar_LF;

    juce::FlexBox playlistThumbnailsFlexBox;
    int nChannels = 0;
	float channelOccupiedSpace = 0.0f;

    juce::OwnedArray<AudioThumbnailChannelPlaylistUI> audioThumbnailChannels;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistThumbnailChannelsView)
};