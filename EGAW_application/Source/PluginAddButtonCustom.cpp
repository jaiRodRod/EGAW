/*
  ==============================================================================

    PluginAddButtonCustom.cpp
    Created: 22 May 2025 4:16:55pm
    Author:  jaezro

  ==============================================================================
*/

#include "PluginAddButtonCustom.h"

PluginAddButtonCustom::PluginAddButtonCustom(juce::ValueTree& projectData, juce::String channelUuid) : juce::TextButton(), projectData(projectData), channelUuid(channelUuid)
{
	setTooltip("Add Plugin");
	setMouseCursor(juce::MouseCursor::PointingHandCursor);
	setButtonText("Add Plugin");
}

PluginAddButtonCustom::~PluginAddButtonCustom()
{
}

void PluginAddButtonCustom::paint (juce::Graphics& g)
{
	auto colourID = projectData.getChildWithName(channelUuid).getProperty("Colour").toString();
	auto baseColour = juce::Colour::fromString(colourID);

	g.fillAll (baseColour);

	if(mouseOver)
	{
		g.setColour (baseColour.darker(0.5f));
	}
	else
	{
		g.setColour (baseColour.darker(0.8f));
	}
	g.fillRoundedRectangle (getLocalBounds().reduced(2).toFloat(), 4.0f);

	g.setColour (juce::Colours::white);
	g.drawText (getButtonText(), getLocalBounds(), juce::Justification::centred, true);
}

void PluginAddButtonCustom::resized()
{
	auto area = getLocalBounds();
	setBounds(area);
}