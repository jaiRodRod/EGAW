/*
  ==============================================================================

    GlobalPlayhead.h
    Created: 16 Mar 2025 7:05:01pm
    Author:  admin

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SignalManagerUI.h"
#include "SharedLock_UI_Timers.h"

class GlobalPlayhead : public juce::Timer
{
public:

    void timerCallback() override;

    void add_numSamples(int numSamples) { globalPlayhead += numSamples; };
    juce::int64 getPlayheadPosition() { return globalPlayhead.load(); };
    void setPlayheadPosition(juce::int64 newPosition) { globalPlayhead.store(newPosition); }

    double getSampleRate() { return sampleRate; };
    void setSampleRate(double newSampleRate) { sampleRate = newSampleRate; };

    double getTimeLengthSeconds() { return time.inSeconds(); };
    void contestForTimeLength(juce::int64 numSamples);

    double getRealAudioTime() { return realAudioTime.inSeconds(); };

    JUCE_DECLARE_SINGLETON(GlobalPlayhead, false);
private:

    GlobalPlayhead();
    ~GlobalPlayhead() { clearSingletonInstance(); };

    std::atomic<juce::int64> globalPlayhead{ 0 };
    double sampleRate;
    juce::RelativeTime time;
    juce::RelativeTime realAudioTime;

};
