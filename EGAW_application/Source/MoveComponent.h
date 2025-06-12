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

#include "CustomMoveButton_LF_01.h"
#include "CustomMoveButton_LF_02.h"
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

	CustomMoveButton_LF_Left customMoveButton_LF_Left;
    MoveLeftButton moveLeftButton;

	CustomMoveButton_LF_Right customMoveButton_LF_Right;
    MoveRightButton moveRightButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoveComponent)
};
