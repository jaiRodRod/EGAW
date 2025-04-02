/*
  ==============================================================================

    PlaylistChannelsView.h
    Created: 13 Mar 2025 5:17:31pm
    Author:  admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ZoomConstants.h"

#include "AudioChannelPlaylistUI.h"
#include "SignalManagerUI.h"
#include "DisplaySingleton.h"
#include "PlaylistThumbnailChannelsView.h"
#include "CustomScrollViewport.h"

//==============================================================================
/*
*/
class PlaylistChannelsView  : public juce::Component, public juce::ValueTree::Listener, public juce::MessageListener
{
public:
    PlaylistChannelsView(juce::ValueTree&, juce::ValueTree&, juce::Viewport&);
    ~PlaylistChannelsView() override;

    void valueTreePropertyChanged (juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override;
    void valueTreeChildAdded (juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded) override;
    void valueTreeChildRemoved (juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override;
    void valueTreeChildOrderChanged (juce::ValueTree& parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex) override;
	void handleMessage(const juce::Message& message) override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    void rebuildUI();

    void doAddAudioChannel();

    AudioChannelPlaylistUI* getAudioChannelPlaylistUI(juce::String);
    int getNumAudioChannels();

    juce::ValueTree projectData;
    juce::ValueTree playheadState;


    juce::FlexBox playlistFlexBox;
    
    juce::OwnedArray<AudioChannelPlaylistUI> audioChannels;

    juce::Component separator;

    int transportViewPosition;
    PlaylistThumbnailChannelsView playlistThumbnailChannelsView;
    CustomScrollViewport horizontalViewport;

    juce::TextButton addChannel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistChannelsView)
};
