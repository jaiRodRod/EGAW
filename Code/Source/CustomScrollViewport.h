/*
  ==============================================================================

    CustomScrollViewport.h
    Created: 15 Mar 2025 7:03:26pm
    Author:  admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class CustomScrollViewport  : public juce::Viewport
{
public:

    //This viewport bypasses the vertical scrolling to the parent viewport.
    CustomScrollViewport(juce::Viewport& parentViewport);

    void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel) override;

private:
    juce::Viewport& parentViewport;
};
