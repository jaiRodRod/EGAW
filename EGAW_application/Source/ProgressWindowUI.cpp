/*
  ==============================================================================

    ProgressWindowUI.cpp
    Created: 28 Feb 2025 10:53:17am
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ProgressWindowUI.h"

//==============================================================================
ProgressWindowUI::ProgressWindowUI(RenderThread& thread, const juce::String& title)
    : DialogWindow(title, juce::Colours::lightgrey, true)
    , thread(thread)
    , progressBar(std::make_unique<juce::ProgressBar>(progressValue))
{
    setContentOwned(new Content(progressBar.get()), true);
    setResizable(false, false);
    centreWithSize(300, 100);
    startTimerHz(30);
}

void ProgressWindowUI::closeButtonPressed()
{
    thread.stopThread(1000);
}

void ProgressWindowUI::timerCallback()
{
    progressValue = progress.load();

    progressValue = thread.getCurrentPosition() / (double)thread.getTotalLength();

    if (!thread.isThreadRunning() || thread.threadShouldExit())
        exitModalState(0); // Close window when done
}
