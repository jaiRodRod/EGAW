/*
  ==============================================================================

    MixerView.cpp
    Created: 22 Feb 2025 11:33:19am
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MixerView.h"

//==============================================================================
MixerView::MixerView(juce::ValueTree& projectData) : projectData(projectData), mixerChannelsView(projectData), masterBusView(projectData)
{
    projectData.addListener(this);

    addAndMakeVisible(masterBusView);

    mixerViewport.setScrollBarPosition(false, true);
    mixerViewport.setScrollBarsShown(false, true, false, true);
    mixerViewport.setViewedComponent(&mixerChannelsView);
    addAndMakeVisible(mixerViewport);

    //setAlwaysOnTop(true);
}

MixerView::~MixerView()
{
    projectData.removeListener(this);
}

void MixerView::valueTreeChildAdded(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded)
{
    if (parentTree == projectData)
    {
        resized();
    }
}

void MixerView::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::red);   // clear the background

}

void MixerView::resized()
{
    auto area = getLocalBounds();

    auto displayBounds = DisplaySingleton::getInstance()->getMainDisplayBounds();

    auto mixerWidth = (displayBounds.getWidth() / 18);
    masterBusView.setBounds(area.removeFromLeft(mixerWidth));

    mixerViewport.setViewPositionProportionately(mixerWidth, 0);
    mixerViewport.setBounds(area);
    //Edit
    mixerChannelsView.setSize(mixerWidth * projectData.getNumChildren(), area.getHeight());
}

/*
void MixerView::setHoverMoveMixerViewport(bool shouldViewportMoveWithHover)
{
    //mixerViewport.auto
}
*/
