/*
  ==============================================================================

    Footer.cpp
    Created: 19 Feb 2025 9:54:56am
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Footer.h"

//==============================================================================
Footer::Footer(juce::ValueTree& projectData) 
    : flexBox(juce::FlexBox::Direction::row, juce::FlexBox::Wrap::noWrap
        , juce::FlexBox::AlignContent::flexStart, juce::FlexBox::AlignItems::flexStart
        , juce::FlexBox::JustifyContent::flexStart)
    , projectData(projectData)
    , playerControlGrid(projectData)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    addAndMakeVisible(playerControlGrid);
}

Footer::~Footer()
{
}

void Footer::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (MENU_BACKGROUND_COLOUR_ID));
}

void Footer::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    auto footerBounds = getLocalBounds();
    //DBG("Footer bounds: " << footerBounds.toString());

    flexBox.items.clear();

    auto playerControlWidth = (float) ((footerBounds.getWidth() / 12) * 5);
    flexBox.items.add(juce::FlexItem(playerControlGrid).withMinHeight((float) footerBounds.getHeight()).withMinWidth(playerControlWidth));

    flexBox.performLayout(footerBounds);
}
