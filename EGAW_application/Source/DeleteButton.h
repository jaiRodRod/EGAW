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
class DeleteButton  : public juce::ImageButton
{
public:
    DeleteButton(juce::ValueTree& projectData, juce::String channelUuid, bool playlistFlag);
    ~DeleteButton() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    void showAcceptPopUp();
    void deleteChannel();

    juce::ValueTree projectData;
    juce::String channelUuid;
	juce::Image buttonImage;
	bool playlistFlag = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeleteButton)
};
