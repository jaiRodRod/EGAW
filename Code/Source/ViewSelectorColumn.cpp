/*
  ==============================================================================

    ViewSelectorColumn.cpp
    Created: 12 Mar 2025 4:57:00pm
    Author:  admin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ViewSelectorColumn.h"

//==============================================================================
ViewSelectorColumn::ViewSelectorColumn(juce::ValueTree& projectData) 
    : flexBox(juce::FlexBox::Direction::column, juce::FlexBox::Wrap::noWrap
        , juce::FlexBox::AlignContent::flexStart, juce::FlexBox::AlignItems::flexStart
        , juce::FlexBox::JustifyContent::flexStart)
    , projectData(projectData)
    , mixerButton()
    , playlistButton()
    , listeningRoomButton()
{

    projectData.addListener(this);

    mixerButton.setButtonText("Mixer");
    mixerButton.onClick = [this] { mixerClick(); };
    addAndMakeVisible(mixerButton);

    playlistButton.setButtonText("Playlist");
    playlistButton.onClick = [this] { playlistClick(); };
    addAndMakeVisible(playlistButton);

    listeningRoomButton.setButtonText("Listening Room");
    listeningRoomButton.onClick = [this] { roomClick(); };
    addAndMakeVisible(listeningRoomButton);

}

ViewSelectorColumn::~ViewSelectorColumn()
{
    projectData.removeListener(this);
}

void ViewSelectorColumn::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
    if (projectData == treeWhosePropertyHasChanged)
    {
        if (property == juce::Identifier("View"))
        {
            if (projectData.getProperty(property) == "MIXER")
            {
                mixerButton.setToggleState(true, juce::NotificationType::dontSendNotification);
                playlistButton.setToggleState(false, juce::NotificationType::dontSendNotification);
                listeningRoomButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            }
            else if (projectData.getProperty(property) == "PLAYLIST")
            {
                mixerButton.setToggleState(false, juce::NotificationType::dontSendNotification);
                playlistButton.setToggleState(true, juce::NotificationType::dontSendNotification);
                listeningRoomButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            }
            else if (projectData.getProperty(property) == "ROOM")
            {
                mixerButton.setToggleState(false, juce::NotificationType::dontSendNotification);
                playlistButton.setToggleState(false, juce::NotificationType::dontSendNotification);
                listeningRoomButton.setToggleState(true, juce::NotificationType::dontSendNotification);
            }
        }
    }
}

void ViewSelectorColumn::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
}

void ViewSelectorColumn::resized()
{
    auto viewSelectorBounds = getLocalBounds();
    //DBG("Footer bounds: " << footerBounds.toString());

    flexBox.items.clear();

    flexBox.items.add(juce::FlexItem(mixerButton).withMinHeight((float)viewSelectorBounds.getHeight()/3).withMinWidth((float)viewSelectorBounds.getWidth()));
    flexBox.items.add(juce::FlexItem(playlistButton).withMinHeight((float)viewSelectorBounds.getHeight()/3).withMinWidth((float)viewSelectorBounds.getWidth()));
    flexBox.items.add(juce::FlexItem(listeningRoomButton).withMinHeight((float)viewSelectorBounds.getHeight()/3).withMinWidth((float)viewSelectorBounds.getWidth()));

    flexBox.performLayout(viewSelectorBounds);

}

void ViewSelectorColumn::mixerClick()
{
    projectData.setProperty("View", "MIXER", nullptr);
}

void ViewSelectorColumn::playlistClick()
{
    projectData.setProperty("View", "PLAYLIST", nullptr);
}

void ViewSelectorColumn::roomClick()
{
    projectData.setProperty("View", "ROOM", nullptr);
}
