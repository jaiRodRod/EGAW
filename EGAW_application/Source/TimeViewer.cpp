/*
  ==============================================================================

    TimeViewer.cpp
    Created: 16 Mar 2025 8:03:19pm
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TimeViewer.h"

//==============================================================================
TimeViewer::TimeViewer(juce::ValueTree& playheadState) : playheadState(playheadState)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    auto finalString = juce::String::formatted("%02d:%02d.%03d", 0, 0, 0);
    timeLabel.setText(finalString, juce::NotificationType::dontSendNotification);
    timeLabel.setEditable(false);
    addAndMakeVisible(timeLabel);

    playheadState.addListener(this);
}

TimeViewer::~TimeViewer()
{
    playheadState.removeListener(this);
}

void TimeViewer::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
    if (treeWhosePropertyHasChanged == playheadState)
    {
        if (property.toString() == "position")
        {
            auto blockPosition = (double)playheadState.getProperty("position");
            auto sampleRate = (double)playheadState.getProperty("sampleRate");
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
    }
}

void TimeViewer::paint (juce::Graphics& g)
{
    g.fillAll (colourConstants::dark_slate_blue_pressed.darker(0.25f));   // clear the background
}

void TimeViewer::resized()
{
    timeLabel.setBounds(getLocalBounds());

    // Dynamically scale the font size to fit the label bounds
    auto bounds = getLocalBounds().toFloat();
    float fontSize = juce::jmin(bounds.getHeight() * 0.8f, bounds.getWidth() * 0.2f); // Adjust scaling factors if needed
    timeLabel.setFont(juce::Font(fontSize));//.withTypefaceStyle("Bold"));
    timeLabel.setJustificationType(juce::Justification::centred);
}
