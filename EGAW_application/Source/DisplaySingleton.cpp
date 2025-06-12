/*
  ==============================================================================

    DisplaySingleton.cpp
    Created: 19 Feb 2025 10:40:16am
    Author:  jaezro

  ==============================================================================
*/

#include "DisplaySingleton.h"

JUCE_IMPLEMENT_SINGLETON(DisplaySingleton);

DisplaySingleton::DisplaySingleton() : displays(juce::Desktop::getInstance().getDisplays()), displayBounds(displays.getTotalBounds(true))
{

}

juce::Displays DisplaySingleton::getDisplays()
{
    return displays;
}

juce::Rectangle<int> DisplaySingleton::getMainDisplayBounds()
{
    return displayBounds;
}

juce::Rectangle<int> DisplaySingleton::getDisplayBounds(juce::Displays::Display display)
{
    return display.totalArea;
}