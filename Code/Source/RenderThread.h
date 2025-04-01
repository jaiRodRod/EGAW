/*
  ==============================================================================

    RenderThread.h
    Created: 28 Feb 2025 10:53:34am
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "AudioSystemBus.h"
#include "SignalManagerUI.h";
#include "GlobalPlayhead.h"

class RenderThread : public juce::Thread, private juce::AsyncUpdater
{
public:

    RenderThread(AudioSystemBus& src, const juce::File& f, double sampleRate, int bufferSize, double seconds_to_render);

    void run() override;

    double getCurrentPosition() const noexcept { return samplesRendered; }
    juce::int64 getTotalLength() const noexcept { return totalSamples; }

private:

    double sampleRate;
    int bufferSize;
    double seconds_to_render;

    AudioSystemBus& audioSource;
    juce::File outputFile;
    juce::int64 samplesRendered{ 0 };
    juce::int64 totalSamples{ 0 };
    juce::String messageToShow;

    bool renderNow();
    void handleAsyncUpdate() override;
};