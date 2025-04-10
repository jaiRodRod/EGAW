/*
  ==============================================================================

    ChannelColourSelector.cpp
    Created: 7 Mar 2025 9:45:46am
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ChannelColourSelector.h"

//==============================================================================
ChannelColourSelector::ChannelColourSelector(juce::ValueTree& channelSettings) : juce::TextButton(), channelSettings(channelSettings)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setButtonText("Open Colour Selector");
    onClick = [this] { openColorSelector(); };
}

ChannelColourSelector::~ChannelColourSelector()
{
    dialogWindow.release();
}

void ChannelColourSelector::openColorSelector()
{
    // Create a ColourSelector component
    auto colourSelector = std::make_unique<ColourPickComponent>(channelSettings);
    colourSelector->setSize(400, 300);

    // Create a dialog window to host the ColourSelector
    juce::DialogWindow::LaunchOptions dialogOptions;
    auto dialogTitle = juce::String("Colour pick: ");
    dialogTitle.append(channelSettings.getProperty("Name"), 500);
    dialogOptions.dialogTitle = dialogTitle;
    dialogOptions.content.setOwned(colourSelector.release());
    //dialogOptions.componentToCentreAround = this;

    //dialogWindow.get()->enterModalState = false;

    // Show the dialog window
    dialogWindow.reset(dialogOptions.create());

    dialogWindow->enterModalState(true, juce::ModalCallbackFunction::create([this](int result) {
        DBG("Dialog closed with result: " << result);
        if (result == 1) // Assuming 1 means "OK" or "Update"
        {
            // Notify the parent that the colour has been updated
            SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
        }
        dialogWindow.release();
        }), true);
}