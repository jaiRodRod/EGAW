/*
  ==============================================================================

    MoveComponent.h
    Created: 6 Mar 2025 3:33:40pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MoveLeftButton.h"
#include "MoveRightButton.h"

//==============================================================================
/*
*/
class MoveComponent  : public juce::Component
{
public:
    MoveComponent(juce::ValueTree& projectData, juce::String channelUuid);
    ~MoveComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::ValueTree projectData;

    //juce::Grid channelVerticalGrid;
    juce::FlexBox flexContainer;
    MoveLeftButton moveLeftButton;
    MoveRightButton moveRightButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoveComponent)
};
