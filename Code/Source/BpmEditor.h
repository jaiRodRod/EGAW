/*
  ==============================================================================

    BpmEditor.h
    Created: 20 Feb 2025 11:52:05am
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <regex>
#include "AudioSystemBus.h"

class BpmEditor : public juce::Component, public juce::ValueTree::Listener
{
public:
    BpmEditor(juce::ValueTree&);
    ~BpmEditor();

    void valueTreePropertyChanged(juce::ValueTree& tree, const juce::Identifier& property) override;
    void resized() override;

private:

    void bpmEditDone();
    void checkValidBpmValueAndSet(juce::String);

    juce::ValueTree projectData;
    juce::String lastBpmValue;
    juce::TextEditor bpmEditor;
    bool initialised = false;
};