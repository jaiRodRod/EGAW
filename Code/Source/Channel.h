/*
  ==============================================================================

    Channel.h
    Created: 9 Feb 2025 11:50:31am
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class RoutableChannel;

class Channel : public juce::PositionableAudioSource
{
public:
    Channel();
    Channel(juce::String&);
    ~Channel();

    void routeTo(RoutableChannel*, bool);         //Encapsular para hacerlo undoable
    void removeRouteTo(RoutableChannel*);   //Encapsular para hacerlo undoable

    juce::String getInternalChannelId();
    void setInternalChannelId(juce::Uuid);
    juce::ValueTree getValueTree();

    juce::Value getMixerPosition();
    void setMixerPosition(int);

private:
    juce::ValueTree channelsRoutedTo;
    juce::Uuid internalChannelId;
    juce::Value mixerPosition;
    //juce::Array<RoutableChannel*> channelsRoutedTo;
};