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
PlaylistView::PlaylistView(juce::ValueTree& projectData, juce::ValueTree& playheadState) 
    : projectData(projectData)
    , playheadState(playheadState)
    , playlistChannelsView(projectData, playheadState, verticalViewport)
{
    projectData.addListener(this);

    addAndMakeVisible(playlistChannelsView);

    
    verticalViewport.setScrollBarPosition(true, false);
    verticalViewport.setScrollBarsShown(true, false, true, false);
    verticalViewport.setViewedComponent(&playlistChannelsView);
    addAndMakeVisible(verticalViewport);
}

PlaylistView::~PlaylistView()
{
    projectData.removeListener(this);
}

void PlaylistView::valueTreeChildAdded(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded)
{
    if (parentTree == projectData)
    {
        SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
    }
}

void PlaylistView::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::purple);   // clear the background
}

void PlaylistView::resized()
{
    auto area = getLocalBounds();

    auto displayBounds = DisplaySingleton::getInstance()->getMainDisplayBounds();

    auto playlistChannelHeight = (displayBounds.getHeight() / 12);

    //verticalViewport.setViewPositionProportionately(playlistChannelHeight, 0);

    playlistChannelsView.setSize(area.getWidth(), (playlistChannelHeight * (getNumAudioChannels() + 1)) + (displayBounds.getHeight() / 36));

    verticalViewport.setBounds(area);
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
