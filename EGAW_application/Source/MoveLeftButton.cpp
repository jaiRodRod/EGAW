/*
  ==============================================================================

    MoveLeftButton.cpp
    Created: 6 Mar 2025 3:34:04pm
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MoveLeftButton.h"

//==============================================================================
MoveLeftButton::MoveLeftButton(juce::ValueTree& projectData, juce::String channelUuid) : juce::TextButton(), projectData(projectData), channelUuid(channelUuid)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	setMouseCursor(juce::MouseCursor::PointingHandCursor);
	setTooltip("Click to move channel left");
    onClick = [this] { moveLeftAction(); };

}

MoveLeftButton::~MoveLeftButton()
{
}

void MoveLeftButton::moveLeftAction()
{
    auto channelIndex = projectData.getChildWithName("channelOrder").indexOf(projectData.getChildWithName("channelOrder").getChildWithName(channelUuid));
    if (channelIndex > 1)
    {
        projectData.getChildWithName("channelOrder").moveChild(channelIndex, channelIndex - 1, nullptr);
    }
}
