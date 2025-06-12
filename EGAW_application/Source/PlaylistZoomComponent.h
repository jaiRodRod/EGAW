/*
  ==============================================================================

    PlaylistZoomComponent.h
    Created: 10 Apr 2025 7:27:50pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ZoomConstants.h"
#include "SignalManagerUI.h"

#include "ColourConstants.h"
#include "CustomZoomButton_LF_01.h"

//==============================================================================
/*
*/
class PlaylistZoomComponent  : public juce::Component, public juce::ValueTree::Listener
{
public:
    PlaylistZoomComponent(juce::ValueTree&);
    ~PlaylistZoomComponent() override;

	void valueTreePropertyChanged (juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void zoomIn();
    juce::String textZoomLabel(double);
    void zoomOut();

private:

	juce::ValueTree projectData; 

	CustomZoomButton_LF zoomInButton_LF{ true };
	CustomZoomButton_LF zoomOutButton_LF{ false };

	juce::FlexBox flexBox;
	juce::TextButton zoomInButton;
	juce::Label zoomLabel;
	juce::TextButton zoomOutButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistZoomComponent)
};
