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
PlaylistThumbnailChannelsView::PlaylistThumbnailChannelsView(juce::ValueTree& projectData)
    : playlistThumbnailsFlexBox(juce::FlexBox::Direction::column, juce::FlexBox::Wrap::noWrap
        , juce::FlexBox::AlignContent::flexStart, juce::FlexBox::AlignItems::flexStart
        , juce::FlexBox::JustifyContent::flexStart)
    , projectData(projectData)
    , transportBoxComponent(projectData)
{
    projectData.addListener(this);
    SignalManagerUI::getInstance()->addListener(this);

    addAndMakeVisible(transportBoxComponent);
}

PlaylistThumbnailChannelsView::~PlaylistThumbnailChannelsView()
{
    projectData.removeListener(this);
    SignalManagerUI::getInstance()->removeListener(this);
}

void PlaylistThumbnailChannelsView::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
    if (treeWhosePropertyHasChanged == projectData)
    {
        if (property.toString() == "Zoom")
        {
            resized();
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
        resized();
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
            resized();
        }
    }
}

void PlaylistThumbnailChannelsView::valueTreeChildOrderChanged(juce::ValueTree& parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex)
{
    if (parentTreeWhoseChildrenHaveMoved == projectData.getChildWithName("channelOrder"))
    {
        resized();
    }
}

void PlaylistThumbnailChannelsView::valueChanged(juce::Value& value)
{
    auto signal = SignalManagerUI::getInstance()->getCurrentSignal();

    switch (signal)
    {
    case SignalManagerUI::Signal::REBUILD_UI:
        rebuildUI();
        break;
    case SignalManagerUI::Signal::RESIZED_TRIGGER:
        repaint();
        resized();
        SignalManagerUI::getInstance()->setSignal(SignalManagerUI::Signal::NULL_SIGNAL);
        break;
    default:
        break;
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
                //Sustituir el 50 por el Coeficiente de Zoom (50 en este caso es que cada segundo de tiempo ocupa 50px)
                auto channelThumbnailWidth = (double)projectData.getProperty("Zoom") * audio->getThumbnailTotalLength();
                audio->setSize(channelThumbnailWidth, channelHeight);
                playlistThumbnailsFlexBox.items.add(juce::FlexItem(*audio).withMinHeight(channelHeight).withMinWidth(channelThumbnailWidth));
            }
        }
        ++numPosition;
    }

    playlistThumbnailsFlexBox.performLayout(area);
    repaint();

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

    SignalManagerUI::getInstance()->setSignal(SignalManagerUI::Signal::RESTORE_UI_PARAMETERS);

    resized();
}

AudioThumbnailChannelPlaylistUI* PlaylistThumbnailChannelsView::getAudioThumbnailChannelPlaylistUI(juce::String channelUuid)
{
    for (int i = 0; i < audioThumbnailChannels.size(); ++i)
    {
        auto* audio = audioThumbnailChannels.getUnchecked(i);
        if (audio->getChannelUuid() == channelUuid)
        {
            return audio;
        }
    }
    return nullptr;
}
