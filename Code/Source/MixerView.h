/*
  ==============================================================================

    MixerView.h
    Created: 22 Feb 2025 11:33:19am
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "DisplaySingleton.h"
#include "MixerChannelsView.h"
#include "MasterBusChannelUI.h"

//==============================================================================
/*
*/
class MixerView  : public juce::Component, public juce::ValueTree::Listener
{
public:
    MixerView(juce::ValueTree&);
    ~MixerView() override;

    void valueTreeChildAdded (juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded) override;

    void paint (juce::Graphics&) override;
    void resized() override;

    //void setHoverMoveMixerViewport(bool);

private:

    juce::ValueTree projectData;

    //MixBusView
    MasterBusChannelUI masterBusView;
    juce::Viewport mixerViewport;
    MixerChannelsView mixerChannelsView;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MixerView)
};
