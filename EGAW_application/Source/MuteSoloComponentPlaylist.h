/*
  ==============================================================================

    MuteSoloComponentPlaylist.h
    Created: 5 Apr 2025 5:56:58pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MuteButton.h"
#include "SoloButton.h"

//==============================================================================
/*
*/
class MuteSoloComponentPlaylist : public juce::Component
{
public:
    MuteSoloComponentPlaylist(juce::ValueTree& projectData, juce::String channelUuid);
    ~MuteSoloComponentPlaylist() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::ValueTree projectData;

    juce::FlexBox flexContainer;
    MuteButton mute;
    SoloButton solo;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MuteSoloComponentPlaylist)
};

