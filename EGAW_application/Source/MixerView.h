/*
  ==============================================================================

    MixerView.h
    Created: 22 Feb 2025 11:33:19am
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <cmath>

#include "CableRoutingViewOverlay.h"
#include "DisplaySingleton.h"
#include "MixerChannelsView.h"
#include "MasterBusChannelUI.h"
#include "CustomScrollBar_LF_01.h"

#include "ColourConstants.h"

#include "ChannelRouteViewSingleton.h"
#include "SignalManagerUI.h"

//==============================================================================
/*
*/
class MixerView  : public juce::Component, public juce::ValueTree::Listener, public juce::MessageListener, public juce::Timer
{
public:
    MixerView(juce::ValueTree&);
    ~MixerView() override;

    void valueTreeChildAdded (juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded) override;

    void paint (juce::Graphics&) override;
    void resized() override;

	void handleMessage(const juce::Message& message) override;
	void timerCallback() override
	{
		if (viewRoutes)
		{
			repaint();
            // SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
		}
	}

private:

	int countPluginRackChannels();

    juce::Point<int> getChannelRouteInButtonCenter(juce::String channelUuid);
    juce::Point<int> getChannelRouteToButtonCenter(juce::String channelUuid);
    bool viewRoutes = false;

    void drawCatenaryCable(juce::Graphics& g,
        juce::Point<float> startPoint,
        juce::Point<float> endPoint,
        float cableThickness,
        juce::Colour cableColour,
        float sagFactor = 1.0f,
        int numSegments = 50);
    void paintRoutingCables(juce::Graphics& g);

    juce::ValueTree projectData;

    //MixBusView
    std::unique_ptr<CableRoutingViewOverlay> cableOverlay;
    MasterBusChannelUI masterBusView;
	CustomScrollBar_LF customScrollBar_LF;
    juce::Viewport mixerViewport;
    MixerChannelsView mixerChannelsView;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MixerView)
};
