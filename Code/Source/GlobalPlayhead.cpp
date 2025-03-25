/*
  ==============================================================================

    GlobalPlayhead.cpp
    Created: 16 Mar 2025 7:05:01pm
    Author:  admin

  ==============================================================================
*/

#include "GlobalPlayhead.h"

JUCE_IMPLEMENT_SINGLETON(GlobalPlayhead);

GlobalPlayhead::GlobalPlayhead() : time(300), realAudioTime(0)
{
    sampleRate = 48000;
    startTimerHz(30);
}

void GlobalPlayhead::timerCallback()
{
    const juce::ScopedLock lock(SharedLock_UI_Timers::lock);

    auto actualTime = juce::RelativeTime(((double)getPlayheadPosition()) / sampleRate);
    if (actualTime.inSeconds() >= getTimeLengthSeconds())
    {
        SignalManagerUI::getInstance()->setSignal(SignalManagerUI::Signal::STOP_AUDIO);
    }
}

void GlobalPlayhead::contestForTimeLength(juce::int64 numSamples)
{
    auto contestantTime = juce::RelativeTime( ((double) numSamples) / sampleRate);

    if (contestantTime.inSeconds() > realAudioTime.inSeconds())
    {
        realAudioTime = (realAudioTime.seconds(contestantTime.inSeconds()));
        time = (time.seconds(contestantTime.inSeconds() + 300)); // 5 minutos de margen
    }
}