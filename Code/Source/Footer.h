/*
  ==============================================================================

    Footer.h
    Created: 19 Feb 2025 9:54:56am
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PlayerControlGrid.h"

#define MENU_BACKGROUND_COLOUR_ID 0

//==============================================================================
/*
*/
class Footer  : public juce::Component
{
public:
    Footer(juce::ValueTree&);
    ~Footer() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::ValueTree& projectData;
    PlayerControlGrid playerControlGrid;
    juce::FlexBox flexBox;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Footer)
};
