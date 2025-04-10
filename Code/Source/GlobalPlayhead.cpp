/*
  ==============================================================================

    GlobalPlayhead.cpp
    Created: 16 Mar 2025 7:05:01pm
    Author:  admin

  ==============================================================================
*/

#include "GlobalPlayhead.h"

GlobalPlayhead::GlobalPlayhead() : playheadState("GlobalPlayhead")
{
    playheadState.setProperty("time", 300, nullptr);
    playheadState.setProperty("realAudioTime", 0, nullptr);
    playheadState.setProperty("position", 0, nullptr);
    playheadState.setProperty("isPlaying", false, nullptr);
    playheadState.setProperty("sampleRate", getSampleRate(), nullptr);
    playheadState.setProperty("bufferSize", getBufferSize(), nullptr);
    startTimerHz(30);
}

void GlobalPlayhead::timerCallback()
{
    auto actualTime = juce::RelativeTime(((double)getPlayheadPosition()) / sampleRate);
    if (actualTime.inSeconds() >= (double)playheadState.getProperty("time"))
    {
        SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::STOP_AUDIO);
    }
}

void GlobalPlayhead::add_numSamples(int numSamples)
{
    globalPlayhead.fetch_add(numSamples);

    juce::MessageManager::callAsync([this] {
        playheadState.setProperty("position", getPlayheadPosition(), nullptr);
    });
}

juce::int64 GlobalPlayhead::getPlayheadPosition() const
{
    return globalPlayhead.load();
}

void GlobalPlayhead::setPlayheadPosition(juce::int64 newPosition)
{
    globalPlayhead.store(newPosition, std::memory_order_release);

    // Update ValueTree (asynchronously, GUI thread safe)
    juce::MessageManager::callAsync([this, newPosition] {
        playheadState.setProperty("position", newPosition, nullptr);
    });
}

double GlobalPlayhead::getSampleRate() const
{
    return sampleRate.load();
}

void GlobalPlayhead::setSampleRate(double newSampleRate)
{
    sampleRate.store(newSampleRate, std::memory_order_release);

    juce::MessageManager::callAsync([this, newSampleRate] {
        playheadState.setProperty("sampleRate", newSampleRate, nullptr);
    });

    DBG("SAMPLE RATE SET: " << newSampleRate);
}

double GlobalPlayhead::getBufferSize() const
{
    return bufferSize.load();
}

void GlobalPlayhead::setBufferSize(double newBufferSize)
{
    bufferSize.store(newBufferSize, std::memory_order_release);

    juce::MessageManager::callAsync([this, newBufferSize] {
        playheadState.setProperty("bufferSize", newBufferSize, nullptr);
    });

    DBG("BUFFER SIZE SET: " << newBufferSize);
}

void GlobalPlayhead::contestForTimeLength(juce::int64 numSamples)
{
    auto contestantTime = juce::RelativeTime( ((double) numSamples) / sampleRate);

    if (contestantTime.inSeconds() > (double)playheadState.getProperty("realAudioTime"))
    {
        juce::MessageManager::callAsync([this, contestantTime] {
            playheadState.setProperty("realAudioTime", contestantTime.inSeconds(), nullptr);
            playheadState.setProperty("time", contestantTime.inSeconds() + 300, nullptr); // 5 minutos de margen
			SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
        });
    }
}

void GlobalPlayhead::setIsPlaying(bool isPlaying)
{
    juce::MessageManager::callAsync([this, isPlaying] {
        playheadState.setProperty("isPlaying", isPlaying, nullptr);
    });
}
