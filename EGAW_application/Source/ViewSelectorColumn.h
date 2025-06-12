/*
  ==============================================================================

    ViewSelectorColumn.h
    Created: 12 Mar 2025 4:57:00pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "ColourConstants.h"
#include "CustomToggleButton_LF_01.h"

//==============================================================================
/*
*/
class ViewSelectorColumn  : public juce::Component, public juce::ValueTree::Listener
{
public:
    ViewSelectorColumn(juce::ValueTree&);
    ~ViewSelectorColumn() override;

    void valueTreePropertyChanged (juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    void mixerClick();
    void playlistClick();
    void roomClick();

	CustomToggleButton_LF customToggleButton_LF;

    juce::ValueTree& projectData;
    juce::ToggleButton mixerButton;
    juce::ToggleButton playlistButton;
    juce::ToggleButton listeningRoomButton;
    juce::FlexBox flexBox;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ViewSelectorColumn)
};
