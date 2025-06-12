/*
  ==============================================================================

    MixerChannelsView.h
    Created: 22 Feb 2025 12:10:19pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "DisplaySingleton.h"
#include "AudioChannelMixerUI.h"
#include "MixBusChannelUI.h"
#include "PluginRackChannelUI.h"
#include "SignalManagerUI.h"

#include "CustomAddButton_LF_02.h"

#include "ColourConstants.h"

//==============================================================================
/*
*/
class MixerChannelsView  : public juce::Component, public juce::ValueTree::Listener, public juce::MessageListener
{
public:
    MixerChannelsView(juce::ValueTree&);
    ~MixerChannelsView() override;

    void valueTreeChildAdded (juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded) override;
    void valueTreeChildRemoved (juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override;
    void valueTreeChildOrderChanged (juce::ValueTree& parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex) override;
	void handleMessage(const juce::Message& message) override;

    void paint (juce::Graphics&) override;
    void resized() override;

	juce::Point<int> getChannelRouteInButtonCenter(juce::String channelUuid);
	juce::Point<int> getChannelRouteToButtonCenter(juce::String channelUuid);

private:

    void rebuildUI();

    void showMenu();
    void doAddAudioChannel();
    void doAddMixBusChannel();
	void doAddPluginRackChannel();

    MixBusChannelUI* getMixBusChannelUI(juce::String);
    PluginRackChannelUI* getPluginRackChannelUI(juce::String);
    AudioChannelMixerUI* getAudioChannelMixerUI(juce::String);

    juce::ValueTree projectData;

    juce::FlexBox mixerFlexBox;
    juce::OwnedArray<MixBusChannelUI> mixBusChannels;
    juce::OwnedArray<PluginRackChannelUI> pluginRackChannels;
    juce::OwnedArray<AudioChannelMixerUI> audioChannels;

	CustomAddButton_LF_02 customAddButton_LF;
    juce::TextButton addChannel;
    juce::PopupMenu addPopUp;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MixerChannelsView)
};
