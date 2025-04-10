/*
  ==============================================================================

    MuteSoloComponentPlaylist.cpp
    Created: 5 Apr 2025 5:56:58pm
    Author:  admin

  ==============================================================================
*/
#include <JuceHeader.h>
#include "MuteSoloComponentPlaylist.h"

//==============================================================================
MuteSoloComponentPlaylist::MuteSoloComponentPlaylist(juce::ValueTree& projectData, juce::String channelUuid)
    : flexContainer(juce::FlexBox::Direction::column, juce::FlexBox::Wrap::noWrap
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

MuteSoloComponentPlaylist::~MuteSoloComponentPlaylist()
{
}

void MuteSoloComponentPlaylist::paint (juce::Graphics& g)
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

void MuteSoloComponentPlaylist::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    auto area = getLocalBounds();

    flexContainer.items.clear();

    flexContainer.items.add(juce::FlexItem(mute).withHeight((float)area.getHeight()).withWidth(area.getWidth()));
    flexContainer.items.add(juce::FlexItem(solo).withHeight((float)area.getHeight()).withWidth(area.getWidth()));

    flexContainer.performLayout(area);

}
