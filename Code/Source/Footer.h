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
#include "ViewSelectorColumn.h"
#include "TimeViewer.h"
#include "PlaylistZoomComponent.h"

#define MENU_BACKGROUND_COLOUR_ID 0

//==============================================================================
/*
*/
class Footer  : public juce::Component
{
public:
    Footer(juce::ValueTree&, juce::ValueTree&);
    ~Footer() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::ValueTree projectData;
    juce::ValueTree playheadState;

    ViewSelectorColumn viewSelectorColumn;
    PlayerControlGrid playerControlGrid;
    TimeViewer timeViewer;
	PlaylistZoomComponent playlistZoomComponent;
    juce::FlexBox flexBox;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Footer)
};
