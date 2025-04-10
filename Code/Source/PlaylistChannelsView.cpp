/*
  ==============================================================================

    PlaylistChannelsView.cpp
    Created: 13 Mar 2025 5:17:31pm
    Author:  admin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistChannelsView.h"

//==============================================================================
PlaylistChannelsView::PlaylistChannelsView(juce::ValueTree& projectData, juce::ValueTree& playheadState)
    : projectData(projectData)
    , playheadState(playheadState)
    , playlistFlexBox(juce::FlexBox::Direction::column, juce::FlexBox::Wrap::noWrap
        , juce::FlexBox::AlignContent::flexStart, juce::FlexBox::AlignItems::flexStart
        , juce::FlexBox::JustifyContent::flexStart)
    , addChannel("Add new audio channel")
{
    projectData.addListener(this);
    playheadState.addListener(this);
    SignalManagerUI::getInstance().addListener(this);

    addChannel.onClick = [this] {doAddAudioChannel();};
    addAndMakeVisible(addChannel);

	rebuildUI();
}

PlaylistChannelsView::~PlaylistChannelsView()
{
    projectData.removeListener(this);
    playheadState.removeListener(this);
    SignalManagerUI::getInstance().removeListener(this);
}

void PlaylistChannelsView::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
    if (treeWhosePropertyHasChanged == projectData)
    {

    }
    else if (treeWhosePropertyHasChanged == playheadState)
    {
        
    }
}

void PlaylistChannelsView::valueTreeChildAdded(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded)
{
    if (parentTree == projectData)
    {
        DBG(childWhichHasBeenAdded.toXmlString());
        auto type = childWhichHasBeenAdded.getProperty("Type");
        if (type.equals("AudioChannel"))
        {
            juce::MessageManager::callAsync([this, uuid = childWhichHasBeenAdded.getType().toString()] {
                audioChannels.add(new AudioChannelPlaylistUI(projectData, uuid));
                addAndMakeVisible(audioChannels.getLast());
                SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
            });
        }
    }
}

void PlaylistChannelsView::valueTreeChildRemoved(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved)
{
    if (parentTree == projectData)
    {
        bool removed = false;
        juce::String childRemovedUuid = childWhichHasBeenRemoved.getType().toString();
        for (AudioChannelPlaylistUI* audioChannelUI : audioChannels)
        {
            if (audioChannelUI->getChannelUuid() == childRemovedUuid)
            {
                audioChannels.removeObject(audioChannelUI, true);
                removed = true;
            }
        }
        if (removed)
        {
            SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
        }
    }
}

void PlaylistChannelsView::valueTreeChildOrderChanged(juce::ValueTree& parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex)
{
    if (parentTreeWhoseChildrenHaveMoved == projectData.getChildWithName("channelOrder"))
    {
        juce::MessageManager::callAsync([this] {
            SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
        });
    }
}

void PlaylistChannelsView::handleMessage(const juce::Message& message)
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

void PlaylistChannelsView::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::green);   // clear the background
}

void PlaylistChannelsView::resized()
{
    setupFlexBoxLayout();
    playlistFlexBox.performLayout(getLocalBounds());
}

void PlaylistChannelsView::rebuildUI()
{
    audioChannels.clear();

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
            audioChannels.add(new AudioChannelPlaylistUI(projectData, channelUuid));
            addAndMakeVisible(audioChannels.getLast());
        }
    }

    resized();
}

void PlaylistChannelsView::doAddAudioChannel()
{
    SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::ADD_AUDIO_CHANNEL);
}

void PlaylistChannelsView::setupFlexBoxLayout()
{
    playlistFlexBox.items.clear();

    if (auto* display = DisplaySingleton::getInstance())
    {
        const float channelHeight = display->getMainDisplayBounds().getHeight() / 12;
        const float channelWidth = getLocalBounds().getWidth();

        // Add audio channels
        for (auto* channel : audioChannels)
        {
            channel->setSize(channelWidth, channelHeight);
            playlistFlexBox.items.add(juce::FlexItem(*channel).withMinHeight(channelHeight)
                .withMinWidth(channelWidth));
        }

        // Add button
        addChannel.setSize(channelWidth, channelHeight);
        playlistFlexBox.items.add(juce::FlexItem(addChannel).withMinHeight(channelHeight).withMinWidth(channelWidth));
    }
}

AudioChannelPlaylistUI* PlaylistChannelsView::getAudioChannelPlaylistUI(juce::String channelUuid)
{
    for (AudioChannelPlaylistUI* audio : audioChannels)
    {
        if (audio->getChannelUuid() == channelUuid)
        {
            return audio;
        }
    }
    return nullptr;
}

int PlaylistChannelsView::getNumAudioChannels()
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
