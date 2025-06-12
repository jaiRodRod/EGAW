/*
  ==============================================================================

    StartSplashWindow.h
    Created: 5 May 2025 7:49:40pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "StartSplashScreen.h"

class StartSplashWindow : public juce::DocumentWindow,
    private juce::Timer
{
public:
    StartSplashWindow(int displayTimeMs = 2000)
        : DocumentWindow("", juce::Colours::black, DocumentWindow::closeButton),
        displayTime(displayTimeMs)
    {
        // Setup window properties
        setUsingNativeTitleBar(false);
		setTitleBarHeight(0);
        setResizable(false, false);
        setAlwaysOnTop(true);

        // Create and set content component
        contentComponent.reset(new StartSplashScreen());
        setContentOwned(contentComponent.get(), true);

        // Set window size to image size
        setSize(800, 400);

        // Start display timer
        startTimer(50);
    }

    void timerCallback() override
    {
        auto elapsed = juce::Time::getMillisecondCounter() - startTime;
        if (elapsed >= displayTime && !isVisible())
        {
            stopTimer();
            closeSplash();
        }
    }

    void closeSplash()
    {
        // Safely remove from desktop
        juce::MessageManager::callAsync([this]()
            {
                removeFromDesktop();
            });
    }

    void show()
    {
        startTime = juce::Time::getMillisecondCounter();
        addToDesktop();
        setVisible(true);
        centreWithSize(getWidth(), getHeight());
    }

private:

    std::unique_ptr<StartSplashScreen> contentComponent;
    juce::uint32 startTime = 0;
    const int displayTime;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StartSplashWindow)
};