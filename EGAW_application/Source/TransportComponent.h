/*
  ==============================================================================

    TransportComponent.h
    Created: 22 Mar 2025 12:14:48pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "ColourConstants.h"

//==============================================================================
/*
*/
class TransportComponent  : public juce::Component
{
public:
    TransportComponent();
    ~TransportComponent() override;

    void mouseDrag (const juce::MouseEvent& event) override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TransportComponent)
};
