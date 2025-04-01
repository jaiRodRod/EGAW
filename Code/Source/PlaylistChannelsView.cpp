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
PlaylistChannelsView::PlaylistChannelsView(juce::ValueTree& projectData, juce::ValueTree& playheadState, juce::Viewport& verticalViewport)
    : projectData(projectData)
    , playheadState(playheadState)
    , playlistFlexBox(juce::FlexBox::Direction::column, juce::FlexBox::Wrap::noWrap
        , juce::FlexBox::AlignContent::flexStart, juce::FlexBox::AlignItems::flexStart
        , juce::FlexBox::JustifyContent::flexStart)
    , separator()
    , playlistThumbnailChannelsView(projectData, playheadState)
    , horizontalViewport(verticalViewport)
    , transportViewPosition(0)
{
    projectData.addListener(this);
    playheadState.addListener(this);
    SignalManagerUI::getInstance()->addListener(this);

    addAndMakeVisible(separator);

    horizontalViewport.setScrollBarPosition(false, true);
    horizontalViewport.setScrollBarsShown(false, true, false, true); //Check
    horizontalViewport.setViewedComponent(&playlistThumbnailChannelsView);
    addAndMakeVisible(horizontalViewport);

    addChannel.setButtonText("Add new Audio Channel");
    addChannel.onClick = [this] {doAddAudioChannel();};
    addAndMakeVisible(addChannel);
}

PlaylistChannelsView::~PlaylistChannelsView()
{
    projectData.removeListener(this);
    playheadState.removeListener(this);
    SignalManagerUI::getInstance()->removeListener(this);
}

void PlaylistChannelsView::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
    if (treeWhosePropertyHasChanged == projectData)
    {
        if (property.toString() == "Zoom")
        {
            SignalManagerUI::getInstance()->setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
        }
    }
    else if (treeWhosePropertyHasChanged == playheadState)
    {
        if (property.toString() == "position")
        {
            auto totalTime = (double)playheadState.getProperty("time");
            auto blockPosition = (double)playheadState.getProperty("position");
            auto sampleRate = (double)playheadState.getProperty("sampleRate");
            auto isPlaying = (bool)playheadState.getProperty("isPlaying");
            juce::RelativeTime time(blockPosition / sampleRate);

            transportViewPosition = (double)projectData.getProperty("Zoom") * time.inSeconds();

            if (horizontalViewport.getViewArea().getWidth() != 0 && isPlaying)
            {
                auto positionX = (transportViewPosition / horizontalViewport.getViewArea().getWidth()) * horizontalViewport.getViewArea().getWidth();
                transportViewPosition = transportViewPosition % horizontalViewport.getViewArea().getWidth();
                if (transportViewPosition <= 5)
                {
                    horizontalViewport.setViewPosition(positionX, 0);
                }
            }

            //DBG("Transport view position: " << transportViewPosition);
        }
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
            audioChannels.add(new AudioChannelPlaylistUI(projectData, (childWhichHasBeenAdded.getType()).toString()));
            addAndMakeVisible(audioChannels.getLast());
        }
        SignalManagerUI::getInstance()->setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
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
            SignalManagerUI::getInstance()->setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
        }
    }
}

void PlaylistChannelsView::valueTreeChildOrderChanged(juce::ValueTree& parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex)
{
    if (parentTreeWhoseChildrenHaveMoved == projectData.getChildWithName("channelOrder"))
    {
        SignalManagerUI::getInstance()->setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
    }
}

void PlaylistChannelsView::valueChanged(juce::Value& value)
{
    auto signal = SignalManagerUI::getInstance()->getCurrentSignal();

    switch (signal)
    {
    case SignalManagerUI::Signal::REBUILD_UI:
        rebuildUI();
        break;
    default:
        break;
    }
}

void PlaylistChannelsView::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::green);   // clear the background
}

void PlaylistChannelsView::resized()
{
    auto area = getLocalBounds();

    playlistFlexBox.items.clear();

    auto displayBounds = DisplaySingleton::getInstance()->getMainDisplayBounds();

    auto channelHeight = (displayBounds.getHeight() / 12);
    auto channelInfoWidth = displayBounds.getWidth() / 12 + area.getWidth() / 12;

    playlistFlexBox.items.add(juce::FlexItem(juce::Component()).withMinHeight(displayBounds.getHeight() / 36).withMinWidth(channelInfoWidth));

    bool masterBusIteration = true;
    int numPosition = 0;
    for (juce::ValueTree channelIdUnderOrder : projectData.getChildWithName("channelOrder"))
    {
        if (masterBusIteration)
        {
            masterBusIteration = false;
        }
        else
        {
            juce::String channelUuid = channelIdUnderOrder.getType().toString();
            juce::ValueTree channel = projectData.getChildWithName(channelUuid);
            channel.setProperty("mixerPosition", numPosition, nullptr);
            auto type = channel.getProperty("Type");
            if (type.equals("AudioChannel"))
            {
                auto* audio = getAudioChannelPlaylistUI(channelUuid);
                if (audio != nullptr)
                {
                    audio->setSize(area.getWidth(), channelHeight);
                    playlistFlexBox.items.add(juce::FlexItem(*audio).withMinHeight(channelHeight).withMinWidth(channelInfoWidth));
                }
            }
        }
        ++numPosition;
    }

    playlistFlexBox.items.add(juce::FlexItem(addChannel).withMinHeight(channelHeight).withMinWidth(channelInfoWidth));

    playlistFlexBox.performLayout(area.removeFromLeft(channelInfoWidth));

    if (area.getWidth() > 5)
    {
        separator.setBounds(area.removeFromLeft(5));
    }

    //Hacemos el mismo procedimiento para los thumnails
    horizontalViewport.setBounds(area);
    //Sustituir el 50 por el Coeficiente de Zoom (50 en este caso es que cada segundo de tiempo ocupa 50px)
    auto thumbnailLengthWidth = (double)projectData.getProperty("Zoom") * (double)playheadState.getProperty("time");
    playlistThumbnailChannelsView.setSize(thumbnailLengthWidth, channelHeight * getNumAudioChannels() + (displayBounds.getHeight() / 36)); //Check
}

void PlaylistChannelsView::rebuildUI()
{
    audioChannels.clear();

    bool masterBusIteration = true;
    for (juce::ValueTree channelIdUnderOrder : projectData.getChildWithName("channelOrder"))
    {
        if (masterBusIteration)
        {
            masterBusIteration = false;
        }
        else
        {
            juce::String channelUuid = channelIdUnderOrder.getType().toString();
            juce::ValueTree channel = projectData.getChildWithName(channelUuid);
            auto type = channel.getProperty("Type");
            if (type.equals("AudioChannel"))
            {
                audioChannels.add(new AudioChannelPlaylistUI(projectData, channelUuid));
                addAndMakeVisible(audioChannels.getLast()); // ¡Importante!
            }
        }
    }

    SignalManagerUI::getInstance()->setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
}

void PlaylistChannelsView::doAddAudioChannel()
{
    SignalManagerUI::getInstance()->setSignal(SignalManagerUI::Signal::ADD_AUDIO_CHANNEL);
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
