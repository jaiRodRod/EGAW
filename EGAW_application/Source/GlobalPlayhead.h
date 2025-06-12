/*
  ==============================================================================

    GlobalPlayhead.h
    Created: 16 Mar 2025 7:05:01pm
    Author:  jaezro

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

    double getBufferSize() const;
    void setBufferSize(double newBufferSize);

    void contestForTimeLength(juce::int64 numSamples);
    juce::ValueTree& getState() { return playheadState; };
    void setIsPlaying(bool);
	bool isPlaying() const { return playheadState.getProperty("isPlaying"); }

    //JUCE_DECLARE_SINGLETON(GlobalPlayhead, false);

    void add_seconds(double seconds);
	void substract_seconds(double seconds);
	

private:

    std::atomic<juce::int64> globalPlayhead{ 0 };
    std::atomic<double> sampleRate { 44100 };
	std::atomic<double> bufferSize{ 512 };

    juce::ValueTree playheadState;

    //juce::RelativeTime time;
    //juce::RelativeTime realAudioTime;

};
