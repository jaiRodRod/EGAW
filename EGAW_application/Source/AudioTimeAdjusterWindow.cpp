/*
  ==============================================================================

    AudioTimeAdjusterWindow.cpp
    Created: 6 Apr 2025 8:29:58pm
    Author:  jaezro

  ==============================================================================
*/

#include "AudioTimeAdjusterWindow.h"

#include "ColourConstants.h"

void showAudioTimeAdjusterPopup(double audioFileStart,
    double audioFileEnd,
    double audioFileLength,
    std::function<void(double, double, bool)> callback)
{
    auto* dialog = new AudioTimeAdjusterWindow(audioFileStart, audioFileEnd, audioFileLength, callback);

    juce::DialogWindow::LaunchOptions options;
    options.content.setOwned(dialog);
    options.dialogTitle = "Adjust Audio File Times";
    options.componentToCentreAround = nullptr;
    options.dialogBackgroundColour = colourConstants::darker_slate_blue_pressed;
    options.escapeKeyTriggersCloseButton = true;
    options.useNativeTitleBar = false;
    options.resizable = false;

    options.launchAsync();
}
