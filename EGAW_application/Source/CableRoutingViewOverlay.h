/*
  ==============================================================================

    CableRoutingViewOverlay.h
    Created: 23 May 2025 6:07:37pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class MixerView; // Forward declaration instead of including MixerView.h

class CableRoutingViewOverlay : public juce::Component
{
public:
    using CableDrawingFunction = std::function<void(juce::Graphics&)>;

    CableRoutingViewOverlay(CableDrawingFunction drawingFunc)
        : drawingFunction(std::move(drawingFunc))
    {
        setInterceptsMouseClicks(false, false);
    }

    void paint(juce::Graphics& g) override
    {
        if (drawingFunction)
            drawingFunction(g);
    }

private:
    CableDrawingFunction drawingFunction;
};