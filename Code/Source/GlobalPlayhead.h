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

class GlobalPlayhead : public juce::Timer
{
public:

    GlobalPlayhead();
    ~GlobalPlayhead() = default;

    void timerCallback() override;

    void add_numSamples(int numSamples);
    juce::int64 getPlayheadPosition() const;
    void setPlayheadPosition(juce::int64 newPosition);

    double getSampleRate() const;
    void setSampleRate(double newSampleRate);

    void contestForTimeLength(juce::int64 numSamples);
    juce::ValueTree& getState() { return playheadState; };
    void setIsPlaying(bool);

    /*
    double getTimeLengthSeconds() { return time.inSeconds(); };

    double getRealAudioTime() { return realAudioTime.inSeconds(); };
    */
    //JUCE_DECLARE_SINGLETON(GlobalPlayhead, false);

private:

    std::atomic<juce::int64> globalPlayhead{ 0 };
    std::atomic<double> sampleRate { 48000 };

    juce::ValueTree playheadState;

    //juce::RelativeTime time;
    //juce::RelativeTime realAudioTime;

};
