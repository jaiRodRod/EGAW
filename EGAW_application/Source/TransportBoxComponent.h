/*
  ==============================================================================

    TransportBoxComponent.h
    Created: 22 Mar 2025 11:36:09am
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ZoomConstants.h"

#include "TransportComponent.h"
#include "GlobalPlayhead.h"

#include "ColourConstants.h"

//==============================================================================
/*
*/
class TransportBoxComponent  : public juce::Component, public juce::ValueTree::Listener
{
public:
    TransportBoxComponent(juce::ValueTree&, juce::ValueTree&, GlobalPlayhead&);
    ~TransportBoxComponent() override;

    void valueTreePropertyChanged (juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override;
    void mouseDoubleClick (const juce::MouseEvent& event) override;
    void mouseDrag (const juce::MouseEvent& event) override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::ValueTree projectData;
    juce::ValueTree playheadState;
	GlobalPlayhead& globalPlayhead;

    int transportViewPosition;
    TransportComponent transportComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TransportBoxComponent)
};
