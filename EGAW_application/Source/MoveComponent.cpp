/*
  ==============================================================================

    MoveComponent.cpp
    Created: 6 Mar 2025 3:33:40pm
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MoveComponent.h"

//==============================================================================
MoveComponent::MoveComponent(juce::ValueTree& projectData, juce::String channelUuid) 
    : flexContainer(juce::FlexBox::Direction::row, juce::FlexBox::Wrap::noWrap
        , juce::FlexBox::AlignContent::flexStart, juce::FlexBox::AlignItems::flexStart
        , juce::FlexBox::JustifyContent::flexStart)
    , projectData(projectData)
    , moveLeftButton(projectData, channelUuid)
    , moveRightButton(projectData, channelUuid)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

	moveLeftButton.setLookAndFeel(&customMoveButton_LF_Left);
    moveLeftButton.setButtonText("Move left");
    addAndMakeVisible(moveLeftButton);

	moveRightButton.setLookAndFeel(&customMoveButton_LF_Right);
    moveRightButton.setButtonText("Move right");
    addAndMakeVisible(moveRightButton);
}

MoveComponent::~MoveComponent()
{
}

void MoveComponent::paint (juce::Graphics& g)
{
    /*
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("MoveComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
    */
}

void MoveComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    auto area = getLocalBounds();

    flexContainer.items.clear();

    flexContainer.items.add(juce::FlexItem(moveLeftButton).withHeight((float)area.getHeight()).withWidth(area.getWidth()));
    flexContainer.items.add(juce::FlexItem(moveRightButton).withHeight((float)area.getHeight()).withWidth(area.getWidth()));

    flexContainer.performLayout(area);

}
