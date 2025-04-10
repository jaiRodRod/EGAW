/*
  ==============================================================================

    DisplaySingleton.h
    Created: 19 Feb 2025 10:40:16am
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class DisplaySingleton
{
public:

    juce::Displays getDisplays();
    juce::Rectangle<int> getMainDisplayBounds();
    juce::Rectangle<int> getDisplayBounds(juce::Displays::Display);

    JUCE_DECLARE_SINGLETON(DisplaySingleton, false);

private:

    DisplaySingleton();
    ~DisplaySingleton() { clearSingletonInstance(); };

    juce::Displays displays;
    juce::Rectangle<int> displayBounds;
};