/*
  ==============================================================================

    MuteSoloComponent.h
    Created: 4 Mar 2025 1:26:15pm
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
class MuteSoloComponent  : public juce::Component
{
public:
    MuteSoloComponent(juce::ValueTree& projectData, juce::String channelUuid);
    ~MuteSoloComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::ValueTree projectData;

    //juce::Grid channelVerticalGrid;
    juce::FlexBox flexContainer;
    MuteButton mute;
    SoloButton solo;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MuteSoloComponent)
};
