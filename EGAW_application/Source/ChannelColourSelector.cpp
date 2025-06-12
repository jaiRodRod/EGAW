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
ChannelColourSelector::ChannelColourSelector(juce::ValueTree& channelSettings) : juce::ImageButton(), channelSettings(channelSettings)
{
    // Load image from binary data (replace "icon_png" with your resource name)
    buttonImage = juce::ImageCache::getFromMemory(BinaryData::palette_icon_png, BinaryData::palette_icon_pngSize);

    // Set up button
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
    setTooltip("Click to select channel color");
    onClick = [this] { openColorSelector(); };

    // Start with hover opacity (0.8)
    opacity = 0.8f;
}

ChannelColourSelector::~ChannelColourSelector()
{
    dialogWindow.release();
}

void ChannelColourSelector::paint(juce::Graphics& g)
{
    if (buttonImage.isValid())
    {
        // Draw image with current opacity
        g.setOpacity(opacity);

        // Scale image to fit while maintaining aspect ratio
        auto bounds = getLocalBounds().toFloat().reduced(5); // Small margin
        juce::RectanglePlacement placement(juce::RectanglePlacement::centred |
            juce::RectanglePlacement::onlyReduceInSize);

        g.drawImage(buttonImage,
            bounds,
            placement,
            false); // Don't fill alpha channel
    }
}

void ChannelColourSelector::resized()
{
	repaint();
}

void ChannelColourSelector::mouseEnter(const juce::MouseEvent&)
{
    // Animate to full opacity on hover
    opacity = 1.0f;
    repaint();
}

void ChannelColourSelector::mouseExit(const juce::MouseEvent&)
{
	// Animate back to hover opacity
	opacity = 0.8f;
	repaint();
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
