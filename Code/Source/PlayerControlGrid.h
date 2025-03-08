/*
  ==============================================================================

    PlayerControlGrid.h
    Created: 19 Feb 2025 4:26:12pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SignalManagerUI.h"
#include "AudioSystemBus.h"
#include "BpmEditor.h"

//==============================================================================
/*
*/
class PlayerControlGrid  : public juce::Component, public juce::Value::Listener
{
public:
    PlayerControlGrid(juce::ValueTree&);
    ~PlayerControlGrid() override;

    void valueChanged(juce::Value&) override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    void setPlaying(bool);
    void playButtonClicked();
    void stopButtonClicked();


    juce::ValueTree& projectData;

    juce::TextButton playButton;
    juce::TextButton stopButton;
    BpmEditor bpmEditor;
    juce::Grid playerGrid;

    bool playing;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayerControlGrid)
};
