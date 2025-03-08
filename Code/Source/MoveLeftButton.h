/*
  ==============================================================================

    MoveLeftButton.h
    Created: 6 Mar 2025 3:34:04pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class MoveLeftButton  : public juce::TextButton
{
public:
    MoveLeftButton(juce::ValueTree& projectData, juce::String channelUuid);
    ~MoveLeftButton() override;

    //void paint (juce::Graphics&) override;
    //void resized() override;

private:

    void moveLeftAction();

    juce::String channelUuid;
    juce::ValueTree projectData;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoveLeftButton)
};
