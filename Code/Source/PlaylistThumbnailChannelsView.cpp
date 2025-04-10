/*
  ==============================================================================

    PlaylistThumbnailChannelsView.cpp
    Created: 13 Mar 2025 5:19:37pm
    Author:  admin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistThumbnailChannelsView.h"

//==============================================================================
PlaylistThumbnailChannelsView::PlaylistThumbnailChannelsView(juce::ValueTree& projectData, juce::ValueTree& playheadState, GlobalPlayhead& globalPlayhead)
    : playlistThumbnailsFlexBox(juce::FlexBox::Direction::column, juce::FlexBox::Wrap::noWrap
        , juce::FlexBox::AlignContent::flexStart, juce::FlexBox::AlignItems::flexStart
        , juce::FlexBox::JustifyContent::flexStart)
    , projectData(projectData)
    , playheadState(playheadState)
	, globalPlayhead(globalPlayhead)
{
    projectData.addListener(this);
    SignalManagerUI::getInstance().addListener(this);
    addMouseListener(this, true);

    rebuildUI();
}

PlaylistThumbnailChannelsView::~PlaylistThumbnailChannelsView()
{
    projectData.removeListener(this);
    SignalManagerUI::getInstance().removeListener(this);
}

void PlaylistThumbnailChannelsView::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
    if (treeWhosePropertyHasChanged == projectData)
    {
        if (property.toString() == "Zoom")
        {
            juce::MessageManager::callAsync([this] {
                SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
            });
        }
    }
    else 
    {
		for (AudioThumbnailChannelPlaylistUI* channel : audioThumbnailChannels)
		{
			if (treeWhosePropertyHasChanged == projectData.getChildWithName(channel->getChannelUuid()))
			{
				if (property == juce::Identifier("StartTime"))
				{
					juce::MessageManager::callAsync([this] {
						resized();
						repaint();  // Also repaint to update the thumbnail drawing
					});
				}
			}
		}
    }
}

void PlaylistThumbnailChannelsView::valueTreeChildAdded(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded)
{
    if (parentTree == projectData)
    {
        DBG(childWhichHasBeenAdded.toXmlString());
        auto type = childWhichHasBeenAdded.getProperty("Type");
        if (type.equals("AudioChannel"))
        {
            juce::MessageManager::callAsync([this, uuid = childWhichHasBeenAdded.getType().toString()] {
                audioThumbnailChannels.add(new AudioThumbnailChannelPlaylistUI(projectData, playheadState, uuid, globalPlayhead));
                addAndMakeVisible(audioThumbnailChannels.getLast());
                SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
            });
        }
    }
}

void PlaylistThumbnailChannelsView::valueTreeChildRemoved(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved)
{
    if (parentTree == projectData)
    {
        bool removed = false;
        juce::String childRemovedUuid = childWhichHasBeenRemoved.getType().toString();
        for (AudioThumbnailChannelPlaylistUI* audioChannelUI : audioThumbnailChannels)
        {
            if (audioChannelUI->getChannelUuid() == childRemovedUuid)
            {
                audioThumbnailChannels.removeObject(audioChannelUI, true);
                removed = true;
            }
        }
        if (removed)
        {
            SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
        }
    }
}

void PlaylistThumbnailChannelsView::valueTreeChildOrderChanged(juce::ValueTree& parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex)
{
    if (parentTreeWhoseChildrenHaveMoved == projectData.getChildWithName("channelOrder"))
    {
        juce::MessageManager::callAsync([this] {
            SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
        });
    }
}

void PlaylistThumbnailChannelsView::handleMessage(const juce::Message& message)
{
    if (auto* signalMsg = dynamic_cast<const SignalMessage*>(&message)) {
        auto signal = static_cast<SignalManagerUI::Signal>(signalMsg->getSignalType());
        // Handle signal (already on message thread)...

        switch (signal)
        {
        case SignalManagerUI::Signal::REBUILD_UI:
            rebuildUI();
            break;
        default:
            break;
        }
    }
}

void PlaylistThumbnailChannelsView::mouseMagnify(const juce::MouseEvent& event, float scaleFactor)
{
	// Handle mouse magnification if needed
	// For example, you can adjust the zoom level based on the scaleFactor
	auto currentZoom = (double)projectData.getProperty("Zoom");
	auto scaledZoom = currentZoom * scaleFactor;

    if (scaledZoom > playlistThumbnailZoomConstants::x0_5 - 1 && scaledZoom < playlistThumbnailZoomConstants::x0_5 + 1)
    {
		scaledZoom = playlistThumbnailZoomConstants::x0_5;
	}
	else if (scaledZoom > playlistThumbnailZoomConstants::x1 - 1 && scaledZoom < playlistThumbnailZoomConstants::x1 + 1)
	{
		scaledZoom = playlistThumbnailZoomConstants::x1;
	}
	else if (scaledZoom > playlistThumbnailZoomConstants::x2 - 1 && scaledZoom < playlistThumbnailZoomConstants::x2 + 1)
	{
		scaledZoom = playlistThumbnailZoomConstants::x2;
	}
	else if (scaledZoom > playlistThumbnailZoomConstants::x5 - 1 && scaledZoom < playlistThumbnailZoomConstants::x5 + 1)
	{
		scaledZoom = playlistThumbnailZoomConstants::x5;
	}
    else if (scaledZoom > playlistThumbnailZoomConstants::x10 - 1 && scaledZoom < playlistThumbnailZoomConstants::x10 + 1)
    {
        scaledZoom = playlistThumbnailZoomConstants::x10;
    }

	projectData.setProperty("Zoom", scaledZoom, nullptr);

	// Rebuild the UI to reflect the new zoom level
	rebuildUI();
}

void PlaylistThumbnailChannelsView::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::blue);   // clear the background
}

void PlaylistThumbnailChannelsView::resized()
{
    setupFlexBoxLayout();
    playlistThumbnailsFlexBox.performLayout(getLocalBounds());
}

void PlaylistThumbnailChannelsView::rebuildUI()
{
    audioThumbnailChannels.clear();

    bool masterBusIteration = true;
    for (auto channelId : projectData.getChildWithName("channelOrder"))
    {
        if (masterBusIteration)
        {
            masterBusIteration = false;
            continue;
        }

        const juce::String channelUuid = channelId.getType().toString();
        auto channel = projectData.getChildWithName(channelUuid);

        if (channel.getProperty("Type") == "AudioChannel")
        {
            audioThumbnailChannels.add(new AudioThumbnailChannelPlaylistUI(projectData, playheadState, channelUuid, globalPlayhead));
            addAndMakeVisible(audioThumbnailChannels.getLast());
        }
    }

    resized();
}

void PlaylistThumbnailChannelsView::setupFlexBoxLayout()
{
    playlistThumbnailsFlexBox.items.clear();

    if (auto* display = DisplaySingleton::getInstance())
    {
        const float channelHeight = display->getMainDisplayBounds().getHeight() / 12;

        for (auto* channel : audioThumbnailChannels)
        {
            /*const float channelWidth = calculateThumbnailWidth(channel->getThumbnailTotalLength());*/
            
            // Get the start time for this channel
            auto channelTree = projectData.getChildWithName(channel->getChannelUuid());
            double startTime = channelTree.getProperty("StartTime", 0.0);
            double zoomFactor = projectData.getProperty("Zoom");
            float startOffset = static_cast<float>(startTime * zoomFactor);

			double audioFileStart = (double)channelTree.getProperty("AudioFileStart", 0.0) * zoomFactor;
			double audioFileEnd = (double)channelTree.getProperty("AudioFileEnd", 0.0) * zoomFactor;
			// channel->setTopLeftPosition(startOffset, 0);
            // channel->setBounds(startOffset, 0, channelWidth, channelHeight);

            playlistThumbnailsFlexBox.items.add(juce::FlexItem(*channel)
                .withMinHeight(channelHeight)
                .withMinWidth(audioFileEnd - audioFileStart)
                .withMargin(juce::FlexItem::Margin( 0, 0, 0, startOffset)));  // Left margin for the offset

        }
    }
}

AudioThumbnailChannelPlaylistUI* PlaylistThumbnailChannelsView::getAudioThumbnailChannelPlaylistUI(juce::String channelUuid)
{
    for (AudioThumbnailChannelPlaylistUI* audio : audioThumbnailChannels)
    {
        if (audio->getChannelUuid() == channelUuid)
        {
            return audio;
        }
    }
    return nullptr;
}

float PlaylistThumbnailChannelsView::calculateThumbnailWidth(double audioThumbnailChannelLength) const
{
	// Original resize calculation
    /*auto channelThumbnailWidth = (double)projectData.getProperty("Zoom") * audio->getThumbnailTotalLength();*/

    const double zoomFactor = projectData.getProperty("Zoom");
    return static_cast<float>(zoomFactor * audioThumbnailChannelLength);
}
