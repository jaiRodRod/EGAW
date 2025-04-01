/*
  ==============================================================================

    PlaylistView.h
    Created: 13 Mar 2025 5:15:45pm
    Author:  admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PlaylistChannelsView.h"
#include "DisplaySingleton.h"
#include "SignalManagerUI.h"

//==============================================================================

class PlaylistView  : public juce::Component, public juce::ValueTree::Listener /*, public juce::Value::Listener*/
{
public:
    PlaylistView(juce::ValueTree&, juce::ValueTree&);
    ~PlaylistView() override;

    /*void valueChanged(juce::Value& value) override;*/
    void valueTreeChildAdded (juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded) override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    int getNumAudioChannels();

    juce::ValueTree projectData;
    juce::ValueTree playheadState;


    juce::Viewport verticalViewport;
    PlaylistChannelsView playlistChannelsView;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistView)
};
