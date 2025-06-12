/*
  ==============================================================================

    ColourConstants.h
    Created: 8 May 2025 6:27:19pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#ifndef COLOUR_CONSTANTS
#define COLOUR_CONSTANTS

namespace colourConstants
{
    static const juce::Colour dark_slate_blue{ juce::Colour(51, 80, 79) };
    static const juce::Colour dark_slate_blue_pressed{ juce::Colour(36, 57, 57) };
    static const juce::Colour darker_slate_blue_pressed{ juce::Colour(31, 45, 45) };
    static const juce::Colour dark_green{ juce::Colour(64, 102, 85) };
	static const juce::Colour darker_green{ juce::Colour(27, 43, 36) };
    static const juce::Colour plain_green{ juce::Colour(103, 135, 113) };
    static const juce::Colour light_green{ juce::Colour(163, 220, 180) };
    static const juce::Colour light_blue{ juce::Colour(137, 177, 187) };
    static const juce::Colour audio_channel_base_colour{ juce::Colour(120,120,120) };//(90, 106, 105) };
    static const juce::Colour mix_bus_channel_base_colour{ juce::Colour(60,92,76) };//(90, 106, 105) };
	static const juce::Colour plugin_channel_base_colour{ juce::Colour(81, 85, 50) };
    static const juce::Colour plain_yellow{ juce::Colour(147, 143, 51) };
    static const juce::Colour light_yellow{ juce::Colour(255, 246, 67) };
    static const juce::Colour plain_red{ juce::Colour(147, 51, 51) };
    static const juce::Colour light_red{ juce::Colour(255, 48, 48) };
}

#endif