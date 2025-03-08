/*
  ==============================================================================

    DeleteButton.h
    Created: 6 Mar 2025 9:50:11am
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class DeleteButton  : public juce::TextButton
{
public:
    DeleteButton(juce::ValueTree& projectData, juce::String channelUuid);
    //~DeleteButton() override;

    //void paint (juce::Graphics&) override;
    //void resized() override;

private:

    void showAcceptPopUp();
    void deleteChannel();

    juce::ValueTree projectData;
    juce::String channelUuid;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeleteButton)
};
