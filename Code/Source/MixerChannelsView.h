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
#include "SignalManagerUI.h"

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

private:

    void rebuildUI();

    void showMenu();
    void doAddAudioChannel();
    void doAddMixBusChannel();

    MixBusChannelUI* getMixBusChannelUI(juce::String);
    AudioChannelMixerUI* getAudioChannelMixerUI(juce::String);

    juce::ValueTree projectData;

    juce::FlexBox mixerFlexBox;
    juce::OwnedArray<MixBusChannelUI> mixBusChannels;
    juce::OwnedArray<AudioChannelMixerUI> audioChannels;

    juce::TextButton addChannel;
    juce::PopupMenu addPopUp;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MixerChannelsView)
};
