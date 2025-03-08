/*
  ==============================================================================

    ColourPickComponent.cpp
    Created: 7 Mar 2025 3:05:24pm
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ColourPickComponent.h"

//==============================================================================
ColourPickComponent::ColourPickComponent(juce::ValueTree& channelSettings)
    : channelSettings(channelSettings)
    , flexBox(juce::FlexBox::Direction::column, juce::FlexBox::Wrap::noWrap
        , juce::FlexBox::AlignContent::flexStart, juce::FlexBox::AlignItems::center
        , juce::FlexBox::JustifyContent::center)
    , colourSelector()
    , buttonFlexBox(juce::FlexBox::Direction::row, juce::FlexBox::Wrap::noWrap
        , juce::FlexBox::AlignContent::flexStart, juce::FlexBox::AlignItems::center
        , juce::FlexBox::JustifyContent::center)
    , updateColour()
    , cancel()
{
    auto colourID = channelSettings.getProperty("Colour").toString();
    colourSelector.setCurrentColour(juce::Colour::fromString(colourID));
    addAndMakeVisible(colourSelector);

    updateColour.setButtonText("Update channel colour");
    updateColour.onClick = [this] { doUpdateColour(); };
    addAndMakeVisible(updateColour);

    cancel.setButtonText("Cancel");
    cancel.onClick = [this] { cancelUpdate(); };
    addAndMakeVisible(cancel);
}

ColourPickComponent::~ColourPickComponent()
{
}

void ColourPickComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
}

void ColourPickComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    auto area = getLocalBounds();

    flexBox.items.clear();

    flexBox.items.add(juce::FlexItem(colourSelector).withHeight((float)area.getHeight() * 3/4).withWidth(area.getWidth()));
    flexBox.items.add(juce::FlexItem(buttonFlexBox).withHeight((float)area.getHeight() * 1/4).withWidth(area.getWidth()));
    
    buttonFlexBox.items.clear();
    buttonFlexBox.items.add(juce::FlexItem(updateColour).withHeight((float)area.getHeight() * 1 / 4).withWidth(area.getWidth() / 2));
    buttonFlexBox.items.add(juce::FlexItem(cancel).withHeight((float)area.getHeight() * 1 / 4).withWidth(area.getWidth() / 2));


    flexBox.performLayout(area);

}

void ColourPickComponent::doUpdateColour()
{
    auto colourID = colourSelector.getCurrentColour().toString();
    channelSettings.setProperty("Colour", colourID, nullptr);
    //SignalManagerUI::getInstance()->setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);

    if (auto* dw = findParentComponentOfClass<juce::DialogWindow>())
    {
        dw->exitModalState(1); // 1 indicates "OK" or "Update"
    }
}

void ColourPickComponent::cancelUpdate()
{
    if (auto* dw = findParentComponentOfClass<juce::DialogWindow>())
    {
        dw->exitModalState(0);
    }
}
