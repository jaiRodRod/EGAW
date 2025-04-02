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
PlaylistThumbnailChannelsView::PlaylistThumbnailChannelsView(juce::ValueTree& projectData, juce::ValueTree& playheadState)
    : playlistThumbnailsFlexBox(juce::FlexBox::Direction::column, juce::FlexBox::Wrap::noWrap
        , juce::FlexBox::AlignContent::flexStart, juce::FlexBox::AlignItems::flexStart
        , juce::FlexBox::JustifyContent::flexStart)
    , projectData(projectData)
    , playheadState(playheadState)
    , transportBoxComponent(projectData, playheadState)
{
    projectData.addListener(this);
    SignalManagerUI::getInstance().addListener(this);

    addAndMakeVisible(transportBoxComponent);
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
            SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
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
            audioThumbnailChannels.add(new AudioThumbnailChannelPlaylistUI(projectData, (childWhichHasBeenAdded.getType()).toString()));
            addAndMakeVisible(audioThumbnailChannels.getLast());
        }
        SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
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
        SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
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

void PlaylistThumbnailChannelsView::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::blue);   // clear the background
}

void PlaylistThumbnailChannelsView::resized()
{
    auto area = getLocalBounds();

    playlistThumbnailsFlexBox.items.clear();

    auto displayBounds = DisplaySingleton::getInstance()->getMainDisplayBounds();

    auto channelHeight = (displayBounds.getHeight() / 12);
    //auto channelThumbnailWidth = displayBounds.getWidth();

    DBG("AREA PASADA: " << juce::String(area.getWidth()) << ", " << juce::String(area.getHeight()));
    transportBoxComponent.setBounds(area.removeFromTop(displayBounds.getHeight() / 36));

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
                auto* audio = getAudioThumbnailChannelPlaylistUI(channelUuid);
                if (audio != nullptr)
                {
                    //Sustituir el 50 por el Coeficiente de Zoom (50 en este caso es que cada segundo de tiempo ocupa 50px)
                    auto channelThumbnailWidth = (double)projectData.getProperty("Zoom") * audio->getThumbnailTotalLength();
                    audio->setSize(channelThumbnailWidth, channelHeight);
                    playlistThumbnailsFlexBox.items.add(juce::FlexItem(*audio).withMinHeight(channelHeight).withMinWidth(channelThumbnailWidth));
                }
            }
        }
        ++numPosition;
    }

    playlistThumbnailsFlexBox.performLayout(area);
    //repaint();

    DBG("ACABA DE FORMARSE EL PlaylistThumbnailsChannelsView");

}

void PlaylistThumbnailChannelsView::rebuildUI()
{
    audioThumbnailChannels.clear();

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
                audioThumbnailChannels.add(new AudioThumbnailChannelPlaylistUI(projectData, channelUuid));
                addAndMakeVisible(audioThumbnailChannels.getLast()); // ¡Importante!
            }
        }
    }

    SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
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
