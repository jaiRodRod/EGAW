/*
  ==============================================================================

    ViewSelectorColumn.cpp
    Created: 12 Mar 2025 4:57:00pm
    Author:  jaezro

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

    mixerButton.setLookAndFeel(&customToggleButton_LF);
    mixerButton.setButtonText("Mixer");
	mixerButton.setToggleable(true);
	mixerButton.setClickingTogglesState(true);
    mixerButton.onClick = [this] { mixerClick(); };
    addAndMakeVisible(mixerButton);

	playlistButton.setLookAndFeel(&customToggleButton_LF);
    playlistButton.setButtonText("Playlist");
	playlistButton.setToggleable(true);
	playlistButton.setClickingTogglesState(true);
    playlistButton.onClick = [this] { playlistClick(); };
    addAndMakeVisible(playlistButton);

	listeningRoomButton.setLookAndFeel(&customToggleButton_LF);
    listeningRoomButton.setButtonText("Listening Room");
	listeningRoomButton.setToggleable(false);
	listeningRoomButton.setClickingTogglesState(false);
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

				mixerButton.setToggleable(false);
				mixerButton.setClickingTogglesState(false);
				playlistButton.setToggleable(true);
				playlistButton.setClickingTogglesState(true);
				listeningRoomButton.setToggleable(true);
				listeningRoomButton.setClickingTogglesState(true);
            }
            else if (projectData.getProperty(property) == "PLAYLIST")
            {
                mixerButton.setToggleState(false, juce::NotificationType::dontSendNotification);
                playlistButton.setToggleState(true, juce::NotificationType::dontSendNotification);
                listeningRoomButton.setToggleState(false, juce::NotificationType::dontSendNotification);

				mixerButton.setToggleable(true);
				mixerButton.setClickingTogglesState(true);
				playlistButton.setToggleable(false);
				playlistButton.setClickingTogglesState(false);
				listeningRoomButton.setToggleable(true);
				listeningRoomButton.setClickingTogglesState(true);
            }
            else if (projectData.getProperty(property) == "ROOM")
            {
                mixerButton.setToggleState(false, juce::NotificationType::dontSendNotification);
                playlistButton.setToggleState(false, juce::NotificationType::dontSendNotification);
                listeningRoomButton.setToggleState(true, juce::NotificationType::dontSendNotification);

				mixerButton.setToggleable(true);
				mixerButton.setClickingTogglesState(true);
				playlistButton.setToggleable(true);
				playlistButton.setClickingTogglesState(true);
				listeningRoomButton.setToggleable(false);
				listeningRoomButton.setClickingTogglesState(false);
			}
			else
			{
				jassertfalse; // Unknown view type
            }
        }
    }
}

void ViewSelectorColumn::paint (juce::Graphics& g)
{
    g.fillAll (colourConstants::dark_slate_blue);   // clear the background
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
