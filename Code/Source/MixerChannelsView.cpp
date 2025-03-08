/*
  ==============================================================================

    MixerChannelsView.cpp
    Created: 22 Feb 2025 12:10:19pm
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MixerChannelsView.h"

//==============================================================================
MixerChannelsView::MixerChannelsView(juce::ValueTree& projectData)
    : mixerFlexBox(juce::FlexBox::Direction::row, juce::FlexBox::Wrap::noWrap
        , juce::FlexBox::AlignContent::flexStart, juce::FlexBox::AlignItems::flexStart
        , juce::FlexBox::JustifyContent::flexStart)
    , projectData(projectData)
{
    projectData.addListener(this);
    SignalManagerUI::getInstance()->addListener(this);

    juce::PopupMenu::Item addAudioChannel("Audio Channel");
    addAudioChannel.setAction([this] { doAddAudioChannel(); });

    juce::PopupMenu::Item addMixBusChannel("Mix Bus Channel");
    addMixBusChannel.setAction([this] { doAddMixBusChannel(); });

    addPopUp.addItem(addAudioChannel);
    addPopUp.addItem(addMixBusChannel);

    addChannel.setButtonText("Add channel");
    addChannel.onClick = [this] {showMenu();};
    addAndMakeVisible(addChannel);

}

MixerChannelsView::~MixerChannelsView()
{
    projectData.removeListener(this);
    SignalManagerUI::getInstance()->removeListener(this);
}

void MixerChannelsView::valueChanged(juce::Value& value)
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

void MixerChannelsView::valueTreeChildAdded(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded)
{
    if (parentTree == projectData)
    {
        DBG(childWhichHasBeenAdded.toXmlString());
        auto type = childWhichHasBeenAdded.getProperty("Type");
        if (type.equals("AudioChannel"))
        {
            audioChannels.add(new AudioChannelMixerUI(projectData, (childWhichHasBeenAdded.getType()).toString()));
            addAndMakeVisible(audioChannels.getLast()); // ¡Importante!
        }
        else if (type.equals("MixBusChannel"))
        {
            mixBusChannels.add(new MixBusChannelUI(projectData, (childWhichHasBeenAdded.getType()).toString()));
            addAndMakeVisible(mixBusChannels.getLast());
        }
        resized();
    }
}

void MixerChannelsView::valueTreeChildRemoved(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved)
{
    if (parentTree == projectData)
    {
        bool removed = false;
        juce::String childRemovedUuid = childWhichHasBeenRemoved.getType().toString();
        for (MixBusChannelUI* mixBusChannelUI : mixBusChannels)
        {
            if (mixBusChannelUI->getChannelUuid() == childRemovedUuid)
            {
                mixBusChannels.removeObject(mixBusChannelUI, true);
                removed = true;
            }
        }
        for (AudioChannelMixerUI* audioChannelUI : audioChannels)
        {
            if (audioChannelUI->getChannelUuid() == childRemovedUuid)
            {
                audioChannels.removeObject(audioChannelUI, true);
                removed = true;
            }
        }
        if (removed)
        {
            resized();
        }
    }
}

void MixerChannelsView::valueTreeChildOrderChanged(juce::ValueTree& parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex)
{
    if (parentTreeWhoseChildrenHaveMoved == projectData.getChildWithName("channelOrder"))
    {
        resized();
    }
}

void MixerChannelsView::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::green);   // clear the background
}

void MixerChannelsView::resized()
{

    auto area = getLocalBounds();

    mixerFlexBox.items.clear();

    auto displayBounds = DisplaySingleton::getInstance()->getMainDisplayBounds();

    auto channelWidth = (displayBounds.getWidth() / 18);

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
                auto* audio = getAudioChannelMixerUI(channelUuid);
                audio->setSize(channelWidth, area.getHeight());
                mixerFlexBox.items.add(juce::FlexItem(*audio).withMinHeight((float)area.getHeight()).withMinWidth(channelWidth));
            }
            else if (type.equals("MixBusChannel"))
            {
                auto* mixer = getMixBusChannelUI(channelUuid);
                mixer->setSize(channelWidth, area.getHeight());
                mixerFlexBox.items.add(juce::FlexItem(*mixer).withMinHeight((float)area.getHeight()).withMinWidth(channelWidth));
            }
        }
        ++numPosition;
    }

    mixerFlexBox.items.add(juce::FlexItem(addChannel).withMinHeight((float)area.getHeight()).withMinWidth(channelWidth));


    mixerFlexBox.performLayout(area);

}


void MixerChannelsView::rebuildUI()
{
    mixBusChannels.clear();
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
                audioChannels.add(new AudioChannelMixerUI(projectData, channelUuid));
                addAndMakeVisible(audioChannels.getLast()); // ¡Importante!
            }
            else if (type.equals("MixBusChannel"))
            {
                mixBusChannels.add(new MixBusChannelUI(projectData, channelUuid));
                addAndMakeVisible(mixBusChannels.getLast());
            }
        }
    }

    SignalManagerUI::getInstance()->setSignal(SignalManagerUI::Signal::RESTORE_UI_PARAMETERS);

    resized();
}

void MixerChannelsView::showMenu()
{
    addPopUp.showMenuAsync(juce::PopupMenu::Options());
}

void MixerChannelsView::doAddAudioChannel()
{
    SignalManagerUI::getInstance()->setSignal(SignalManagerUI::Signal::ADD_AUDIO_CHANNEL);
}

void MixerChannelsView::doAddMixBusChannel()
{
    SignalManagerUI::getInstance()->setSignal(SignalManagerUI::Signal::ADD_MIX_BUS_CHANNEL);
}

MixBusChannelUI* MixerChannelsView::getMixBusChannelUI(juce::String channelUuid)
{
    for (int i = 0; i < mixBusChannels.size(); ++i)
    {
        auto* mixBus = mixBusChannels.getUnchecked(i);
        if (mixBus->getChannelUuid() == channelUuid)
        {
            return mixBus;
        }
    }
    return nullptr;
}

AudioChannelMixerUI* MixerChannelsView::getAudioChannelMixerUI(juce::String channelUuid)
{
    for (int i = 0; i < audioChannels.size(); ++i)
    {
        auto* audio = audioChannels.getUnchecked(i);
        if (audio->getChannelUuid() == channelUuid)
        {
            return audio;
        }
    }
    return nullptr;
}
