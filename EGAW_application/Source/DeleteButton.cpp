/*
  ==============================================================================

    DeleteButton.cpp
    Created: 6 Mar 2025 9:50:11am
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeleteButton.h"

//==============================================================================
DeleteButton::DeleteButton(juce::ValueTree& projectData, juce::String channelUuid, bool playlistFlag) : juce::ImageButton(), projectData(projectData), channelUuid(channelUuid), playlistFlag(playlistFlag)
{
    buttonImage = juce::ImageCache::getFromMemory(BinaryData::delete_icon_png, BinaryData::delete_icon_pngSize);

    // Set up button
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
    setTooltip("Click to delete channel");
    onClick = [this] {showAcceptPopUp();};
}

void DeleteButton::showAcceptPopUp()
{
    bool result = juce::AlertWindow::showOkCancelBox(
        juce::AlertWindow::QuestionIcon,
        "Delete channel",
        "¿Are you sure you want to delete the channel?",
        "Yes",
        "No",
        nullptr,
        juce::ModalCallbackFunction::create([this](int result)
            {
                if (result == 1)
                {
                    // Handle deletion here
                    deleteChannel();
                }
            })
    );
}

void DeleteButton::deleteChannel()
{
    DBG("DELETING CHANNEL");
    projectData.getChildWithName("channelOrder").removeChild(
        projectData.getChildWithName("channelOrder").getChildWithName(channelUuid)
        , nullptr
    );
    projectData.removeChild(projectData.getChildWithName(channelUuid), nullptr);
}


DeleteButton::~DeleteButton()
{
}

void DeleteButton::paint (juce::Graphics& g)
{
    if (buttonImage.isValid())
    {
        // Draw image with current opacity
        g.setOpacity(0.8);

        // Scale image to fit while maintaining aspect ratio
        auto bounds = getLocalBounds().toFloat().reduced(5); // Small margin
		if (playlistFlag) bounds.setX(bounds.getX() - 4);
        juce::RectanglePlacement placement(juce::RectanglePlacement::centred |
            juce::RectanglePlacement::onlyReduceInSize);

        g.drawImage(buttonImage,
            bounds,
            placement,
            false); // Don't fill alpha channel
    }
}

void DeleteButton::resized()
{
	repaint();
}