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
Footer::Footer(juce::ValueTree& projectData, juce::ValueTree& playheadState) 
    : flexBox(juce::FlexBox::Direction::row, juce::FlexBox::Wrap::noWrap
        , juce::FlexBox::AlignContent::flexStart, juce::FlexBox::AlignItems::center
        , juce::FlexBox::JustifyContent::flexStart)
    , projectData(projectData)
    , playheadState(playheadState)
    , viewSelectorColumn(projectData)
    , playerControlGrid(projectData, playheadState)
    , timeViewer(playheadState)
	, playlistZoomComponent(projectData)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    addAndMakeVisible(viewSelectorColumn);

    addAndMakeVisible(playerControlGrid);

    addAndMakeVisible(timeViewer);

	addAndMakeVisible(playlistZoomComponent);

	projectData.addListener(this);
}

Footer::~Footer()
{
	projectData.removeListener(this);
}

void Footer::paint (juce::Graphics& g)
{
    g.fillAll (colourConstants::dark_slate_blue);
}

void Footer::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    auto footerBounds = getLocalBounds();
    //DBG("Footer bounds: " << footerBounds.toString());

    flexBox.items.clear();

    auto viewSelectorWidth = (float)((footerBounds.getWidth() / 12) * 1.333f);
    auto playerControlWidth = (float)((footerBounds.getWidth() / 12) * 1.333f);

    flexBox.items.add(juce::FlexItem(juce::FlexItem().withMinWidth(((footerBounds.getWidth() / 12) / 12))));

    flexBox.items.add(juce::FlexItem(playerControlGrid).withMinHeight((float) footerBounds.getHeight() * 0.666f).withMinWidth(playerControlWidth));

    flexBox.items.add(juce::FlexItem(juce::FlexItem().withMinWidth(((footerBounds.getWidth() / 12) / 12))));

    flexBox.items.add(juce::FlexItem(timeViewer).withMinHeight((float)footerBounds.getHeight()).withMinWidth(viewSelectorWidth));

    // flexBox.items.add(juce::FlexItem(juce::FlexItem().withMinWidth(((footerBounds.getWidth() / 12) / 12))));

    flexBox.items.add(juce::FlexItem(viewSelectorColumn).withMinHeight((float)footerBounds.getHeight()).withMinWidth(viewSelectorWidth));

    flexBox.items.add(juce::FlexItem(juce::FlexItem().withMinWidth(((footerBounds.getWidth() / 12) / 12))));

    if (projectData.getProperty("View").toString() == "PLAYLIST")
    {
        playlistZoomComponent.setVisible(true);
        flexBox.items.add(juce::FlexItem(playlistZoomComponent).withMinHeight((float)footerBounds.getHeight()).withMinWidth(viewSelectorWidth / 2.0f));
    }
    else 
    {
		playlistZoomComponent.setVisible(false);
    }

    flexBox.performLayout(footerBounds);
}

void Footer::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
	if (treeWhosePropertyHasChanged == projectData)
	{
		if (property.toString() == "View")
		{
			resized();
		}
	}
}
