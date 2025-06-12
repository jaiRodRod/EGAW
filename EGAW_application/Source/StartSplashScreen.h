/*
  ==============================================================================

    StartSplashScreen.h
    Created: 5 May 2025 7:10:13pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "BinaryData.h" // Include your binary data header for image resources

class StartSplashScreen : public juce::Component,
    private juce::Timer
{
public:
    StartSplashScreen()
    {
        // Load your splash image (replace with your actual image resource)
		splashImage = loadSplashImage();

        if (splashImage.isNull())
        {
            // Fallback if image loading fails
            splashImage = juce::Image(juce::Image::RGB, 400, 300, true);
            juce::Graphics g(splashImage);
            g.fillAll(juce::Colours::white);
            g.setColour(juce::Colours::black);
            g.setFont(20.0f);
            g.drawText("Loading...", getLocalBounds(), juce::Justification::centred, false);
        }

        setSize(splashImage.getWidth(), splashImage.getHeight());
        startTimer(50); // Check every 50ms
    }

    void paint(juce::Graphics& g) override
    {
        g.drawImage(splashImage, getLocalBounds().toFloat());
    }

    void setComplete()
    {
        initializationComplete = true;
    }

private:
    void timerCallback() override
    {
        if (initializationComplete)
        {
            stopTimer();
            juce::MessageManager::callAsync([this]()
                {
                    if (auto* dw = dynamic_cast<juce::DialogWindow*>(getParentComponent()))
                        dw->exitModalState(0);
                });
        }
    }

	juce::Image loadSplashImage()
	{
        int imageSize;
		const char* imageData = BinaryData::EGAW_main_png;
        imageSize = BinaryData::EGAW_main_pngSize;

		return juce::ImageCache::getFromMemory(imageData, imageSize);
	}

    juce::Image splashImage;
    std::atomic<bool> initializationComplete{ false };

};