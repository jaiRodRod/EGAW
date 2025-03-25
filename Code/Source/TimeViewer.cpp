/*
  ==============================================================================

    TimeViewer.cpp
    Created: 16 Mar 2025 8:03:19pm
    Author:  admin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TimeViewer.h"

//==============================================================================
TimeViewer::TimeViewer()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    timeLabel.setEditable(false);
    addAndMakeVisible(timeLabel);

    startTimerHz(30);
}

TimeViewer::~TimeViewer()
{
    stopTimer();
}

void TimeViewer::timerCallback()
{
    const juce::ScopedLock lock(SharedLock_UI_Timers::lock);

    auto blockPosition = GlobalPlayhead::getInstance()->getPlayheadPosition();
    auto sampleRate = GlobalPlayhead::getInstance()->getSampleRate();
    juce::RelativeTime time(blockPosition / sampleRate);

    auto ms = time.inMilliseconds();
    ms = ms % 1000;

    auto s = time.inSeconds();
    s = fmod(s, 60);
    auto seconds = juce::int64(s);

    auto m = time.inMinutes();
    m = fmod(m, 60);
    auto minutes = juce::int64(m);

    auto finalString = juce::String::formatted("%02d:%02d.%03d", minutes, seconds, ms);
    timeLabel.setText(finalString, juce::NotificationType::dontSendNotification);
}

void TimeViewer::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
}

void TimeViewer::resized()
{
    timeLabel.setBounds(getLocalBounds());
}
