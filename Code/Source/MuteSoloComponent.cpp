/*
  ==============================================================================

    MuteSoloComponent.cpp
    Created: 4 Mar 2025 1:26:15pm
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MuteSoloComponent.h"

//==============================================================================
MuteSoloComponent::MuteSoloComponent(juce::ValueTree& projectData, juce::String channelUuid)
    : flexContainer(juce::FlexBox::Direction::row, juce::FlexBox::Wrap::noWrap
        , juce::FlexBox::AlignContent::flexStart, juce::FlexBox::AlignItems::flexStart
        , juce::FlexBox::JustifyContent::flexStart)
    , projectData(projectData)
    , mute(projectData.getChildWithName(channelUuid))
    , solo(projectData.getChildWithName(channelUuid))
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    mute.setButtonText("Mute");
    addAndMakeVisible(mute);

    solo.setButtonText("Solo");
    addAndMakeVisible(solo);

}

MuteSoloComponent::~MuteSoloComponent()
{
}

void MuteSoloComponent::paint (juce::Graphics& g)
{
    /*
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("MuteSoloComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
    */
}

void MuteSoloComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    auto area = getLocalBounds();

    flexContainer.items.clear();

    flexContainer.items.add(juce::FlexItem(mute).withHeight((float)area.getHeight()).withWidth(area.getWidth()));
    flexContainer.items.add(juce::FlexItem(solo).withHeight((float)area.getHeight()).withWidth(area.getWidth()));

    flexContainer.performLayout(area);

}
