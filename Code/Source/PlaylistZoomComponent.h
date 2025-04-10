/*
  ==============================================================================

    PlaylistZoomComponent.h
    Created: 10 Apr 2025 7:27:50pm
    Author:  admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ZoomConstants.h"
#include "SignalManagerUI.h"

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

private:

    void zoomIn();
    juce::String textZoomLabel(double);
    void zoomOut();

	juce::ValueTree projectData; 

	juce::FlexBox flexBox;
	juce::TextButton zoomInButton;
	juce::Label zoomLabel;
	juce::TextButton zoomOutButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistZoomComponent)
};
