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

#include "PlayPauseButton.h"
#include "StopButton.h"

//==============================================================================
/*
*/
class PlayerControlGrid  : public juce::Component, public juce::ValueTree::Listener
{
public:
    PlayerControlGrid(juce::ValueTree&, juce::ValueTree&);
    ~PlayerControlGrid() override;

    //void valueChanged(juce::Value&) override;
    void valueTreePropertyChanged (juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void playButtonClicked();
    void stopButtonClicked();

private:

    void setPlaying(bool);



    juce::ValueTree projectData;
    juce::ValueTree playheadState;

    PlayPauseButton playButton;
    StopButton stopButton;
    /*BpmEditor bpmEditor;*/
    juce::Grid playerGrid;

    bool playing;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayerControlGrid)
};
