/*
  ==============================================================================

    ProgressWindowUI.h
    Created: 28 Feb 2025 10:53:17am
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RenderThread.h"

//==============================================================================
/*
*/
class ProgressWindowUI  : public juce::DialogWindow, private juce::Timer
{
public:
    ProgressWindowUI(RenderThread& thread, const juce::String& title);

    void closeButtonPressed() override;

private:

    RenderThread& thread;
    std::atomic<double> progress{ 0 };
    double progressValue{ 0 };
    std::unique_ptr<juce::ProgressBar> progressBar;

    void timerCallback() override;

    class Content : public juce::Component
    {
    public:
        Content(juce::ProgressBar* bar) : bar(bar)
        {
            if (bar != nullptr)
            {
                addAndMakeVisible(bar);
                setSize(300, 100);
            }
        }

        void resized() override
        {
            if (bar != nullptr)
            {
                auto bounds = getLocalBounds().reduced(20);
                bar->setBounds(bounds.removeFromTop(25));
            }
        }

        juce::ProgressBar* bar;
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProgressWindowUI)
};
