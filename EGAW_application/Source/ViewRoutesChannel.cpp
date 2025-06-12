/*
  ==============================================================================

    ViewRoutesChannel.cpp
    Created: 23 May 2025 5:09:48pm
    Author:  jaezro

  ==============================================================================
*/

#include "ViewRoutesChannel.h"

ViewRoutesChannel::ViewRoutesChannel(juce::String channelUuid) :
	juce::TextButton("View Routes"),
	channelUuid(channelUuid)
{
	//setLookAndFeel(&customAddButton_LF);
	setButtonText("View Routes of this Channel");
	setTooltip("View Routes of this Channel");
	onClick = [this] { viewRoutes(); };

	eyeOpen = juce::Drawable::createFromImageData(BinaryData::eyeOpen_svg, BinaryData::eyeOpen_svgSize);
	eyeClosed = juce::Drawable::createFromImageData(BinaryData::eyeClosed_svg, BinaryData::eyeClosed_svgSize);

	ChannelRouteViewSingleton::getInstance().addListener(this);
}

ViewRoutesChannel::~ViewRoutesChannel()
{
	ChannelRouteViewSingleton::getInstance().removeListener(this);
}

void ViewRoutesChannel::paint(juce::Graphics& g)
{
	if (!viewing && eyeOpen != nullptr)
	{
		eyeOpen->replaceColour(juce::Colours::black, juce::Colours::black.withAlpha(0.8f));
		eyeOpen->setTransformToFit(getLocalBounds().reduced(9).toFloat(), juce::RectanglePlacement::centred);
		eyeOpen->draw(g, 1.0f);
	}
	else if (viewing && eyeClosed != nullptr && ChannelRouteViewSingleton::getInstance().getChannelUuid() == channelUuid)
	{
		eyeClosed->replaceColour(juce::Colours::black, juce::Colours::black.withAlpha(0.8f));
		eyeClosed->setTransformToFit(getLocalBounds().reduced(5).toFloat(), juce::RectanglePlacement::centred);
		eyeClosed->draw(g, 1.0f);
	}
}

void ViewRoutesChannel::viewRoutes()
{
	if (viewing && channelUuid == ChannelRouteViewSingleton::getInstance().getChannelUuid())
	{
		viewing = false;
		ChannelRouteViewSingleton::getInstance().setState(ChannelRouteViewSingleton::RouteViewState::VIEWING_OFF);
	}
	else
	{
		viewing = true;
		ChannelRouteViewSingleton::getInstance().setChannelUuid(channelUuid);
		ChannelRouteViewSingleton::getInstance().setState(ChannelRouteViewSingleton::RouteViewState::VIEWING_ON);
	}
}
