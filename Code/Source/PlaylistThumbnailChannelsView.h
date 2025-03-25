/*
  ==============================================================================

    PlaylistThumbnailChannelsView.h
    Created: 13 Mar 2025 5:19:37pm
    Author:  admin

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

//==============================================================================
/*
*/
class PlaylistThumbnailChannelsView  : public juce::Component, public juce::ValueTree::Listener, public juce::Value::Listener
{
public:
    PlaylistThumbnailChannelsView(juce::ValueTree&);
    ~PlaylistThumbnailChannelsView() override;

    void valueTreePropertyChanged (juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override;
    void valueTreeChildAdded (juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded) override;
    void valueTreeChildRemoved (juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override;
    void valueTreeChildOrderChanged (juce::ValueTree& parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex) override;
    void valueChanged(juce::Value& value) override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    void rebuildUI();

    AudioThumbnailChannelPlaylistUI* getAudioThumbnailChannelPlaylistUI(juce::String);

    juce::ValueTree& projectData;

    juce::FlexBox playlistThumbnailsFlexBox;

    TransportBoxComponent transportBoxComponent;
    juce::OwnedArray<AudioThumbnailChannelPlaylistUI> audioThumbnailChannels;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistThumbnailChannelsView)
};
