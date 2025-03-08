/*
  ==============================================================================

    MoveRightButton.h
    Created: 6 Mar 2025 3:34:16pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class MoveRightButton  : public juce::TextButton
{
public:
    MoveRightButton(juce::ValueTree& projectData, juce::String channelUuid);
    ~MoveRightButton() override;

    //void paint (juce::Graphics&) override;
    //void resized() override;

private:

    void moveRightAction();

    juce::String channelUuid;
    juce::ValueTree projectData;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoveRightButton)
};
