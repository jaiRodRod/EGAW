/*
  ==============================================================================

    ViewRoutesChannel.h
    Created: 23 May 2025 5:09:48pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "ChannelRouteViewSingleton.h"

#include "BinaryData.h"

class ViewRoutesChannel : public juce::TextButton, public juce::MessageListener
{
public:
	ViewRoutesChannel(juce::String channelUuid);
	~ViewRoutesChannel() override;

	void paint(juce::Graphics& g) override;

	void handleMessage(const juce::Message& message) override
	{
		if (const auto* viewMsg = dynamic_cast<const ChannelRouteViewMessage*>(&message))
		{
			const auto state = static_cast<ChannelRouteViewSingleton::RouteViewState>(viewMsg->viewState);
			if (state == ChannelRouteViewSingleton::RouteViewState::VIEWING_ON)
			{
				viewing = true;
			}
			else if (state == ChannelRouteViewSingleton::RouteViewState::VIEWING_OFF)
			{
				viewing = false;
			}
		}
	};
	
private:

	void viewRoutes();

	std::unique_ptr<juce::Drawable> eyeOpen;
	std::unique_ptr<juce::Drawable> eyeClosed;

	bool viewing = false;
	juce::String channelUuid;
};