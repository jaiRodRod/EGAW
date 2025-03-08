/*
  ==============================================================================

    MoveRightButton.cpp
    Created: 6 Mar 2025 3:34:16pm
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MoveRightButton.h"

//==============================================================================
MoveRightButton::MoveRightButton(juce::ValueTree& projectData, juce::String channelUuid) : juce::TextButton(), projectData(projectData), channelUuid(channelUuid)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    onClick = [this] { moveRightAction(); };
}

MoveRightButton::~MoveRightButton()
{
}

/*
void MoveRightButton::paint (juce::Graphics& g)
{
    
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("MoveRightButton", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
    
}

void MoveRightButton::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
*/

void MoveRightButton::moveRightAction()
{
    auto channelIndex = projectData.getChildWithName("channelOrder").indexOf(projectData.getChildWithName("channelOrder").getChildWithName(channelUuid));
    if (channelIndex < projectData.getChildWithName("channelOrder").getNumChildren() - 1)
    {
        projectData.getChildWithName("channelOrder").moveChild(channelIndex, channelIndex + 1, nullptr);
    }
}
