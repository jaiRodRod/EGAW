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

	mixerViewport.setLookAndFeel(&customScrollBar_LF);
    mixerViewport.setScrollBarPosition(false, true);
    mixerViewport.setScrollBarsShown(false, true, false, true);
    mixerViewport.setViewedComponent(&mixerChannelsView);
    addAndMakeVisible(mixerViewport);

    cableOverlay = std::make_unique<CableRoutingViewOverlay>(
        [this](juce::Graphics& g) {
            if (viewRoutes) paintRoutingCables(g);
        });
    addAndMakeVisible(cableOverlay.get());

	ChannelRouteViewSingleton::getInstance().addListener(this);

    //setAlwaysOnTop(true);
}

MixerView::~MixerView()
{
    projectData.removeListener(this);
	ChannelRouteViewSingleton::getInstance().removeListener(this);
	stopTimer();
}

void MixerView::valueTreeChildAdded(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded)
{
    if (parentTree == projectData)
    {
		SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
    }
}

void MixerView::paint (juce::Graphics& g)
{
    g.fillAll (colourConstants::plain_green);   // clear the background
}

void MixerView::resized()
{
    auto area = getLocalBounds();

    auto displayBounds = DisplaySingleton::getInstance()->getMainDisplayBounds();

    auto mixerWidth = (displayBounds.getWidth() / 18);
    masterBusView.setBounds(area.removeFromLeft(mixerWidth));

    mixerViewport.setViewPositionProportionately(mixerWidth, 0);
    mixerViewport.setBounds(area);
    mixerViewport.getHorizontalScrollBar().setSize(mixerViewport.getWidth(), customScrollBar_LF.getDefaultScrollbarWidth());

    //Edit
    
	// Function to count plugin rack channels
    mixerChannelsView.setSize(mixerWidth * projectData.getNumChildren() + mixerWidth * countPluginRackChannels(), area.getHeight());

    //mixerChannelsView.setSize(mixerWidth * projectData.getNumChildren(), area.getHeight());

    if (cableOverlay)
        cableOverlay->setBounds(getLocalBounds());
}

void MixerView::handleMessage(const juce::Message& message)
{
	if (const auto* viewMsg = dynamic_cast<const ChannelRouteViewMessage*>(&message))
	{
		const auto state = static_cast<ChannelRouteViewSingleton::RouteViewState>(viewMsg->viewState);
		switch (state)
		{
		case ChannelRouteViewSingleton::RouteViewState::VIEWING_ON:
			viewRoutes = true;
			SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
            startTimerHz(30);
			break;
		case ChannelRouteViewSingleton::RouteViewState::VIEWING_OFF:
			viewRoutes = false;
			SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
			stopTimer();
			break;
		default:
			break;
		}
	}
}

int MixerView::countPluginRackChannels()
{
	int pluginRackChannelsCount = 0;
	auto channelOrder = projectData.getChildWithName("channelOrder");
	for (int i = 0; i < channelOrder.getNumChildren(); ++i)
	{
		auto channelData = projectData.getChildWithName(channelOrder.getChild(i).getType());
		if (channelData.getProperty("Type").toString() == "PluginRackChannel")
		{
			pluginRackChannelsCount++;
		}
	}
    return pluginRackChannelsCount;
}

juce::Point<int> MixerView::getChannelRouteInButtonCenter(juce::String channelUuid)
{
    if (channelUuid == juce::Uuid("MasterBusChannel").toString())
    {
        return cableOverlay->getLocalPoint(&masterBusView, masterBusView.getRouteInButtonCenter());
    }
    else
    {
        auto channelPoint = mixerChannelsView.getChannelRouteInButtonCenter(channelUuid);
        return cableOverlay->getLocalPoint(&mixerChannelsView, channelPoint);
    }
}

juce::Point<int> MixerView::getChannelRouteToButtonCenter(juce::String channelUuid)
{
    auto channelPoint = mixerChannelsView.getChannelRouteToButtonCenter(channelUuid);
    return cableOverlay->getLocalPoint(&mixerChannelsView, channelPoint);
}

void MixerView::drawCatenaryCable(juce::Graphics& g, juce::Point<float> startPoint, juce::Point<float> endPoint, float cableThickness, juce::Colour cableColour, float sagFactor, int numSegments)
{
    // Calculate differences
    const float dx = endPoint.x - startPoint.x;
    const float dy = endPoint.y - startPoint.y;
    const float length = std::sqrt(dx * dx + dy * dy);

    // For very short distances, draw straight
    if (length < 10.0f)
    {
        g.setColour(cableColour);
        g.drawLine(juce::Line<float>(startPoint, endPoint), cableThickness);
        return;
    }

    // Create a quadratic Bézier path for smooth parabola-like curve
    juce::Path cablePath;
    cablePath.startNewSubPath(startPoint);

    // Control point determines the curve "height"
    const float controlYOffset = length * 0.2f * sagFactor; // Adjust sag
    const juce::Point<float> controlPoint(
        (startPoint.x + endPoint.x) * 0.5f,
		(startPoint.y + controlYOffset)
    );

    // Quadratic Bézier curve (perfect parabola)
    cablePath.quadraticTo(controlPoint, endPoint);

    // Draw outline
    g.setColour(juce::Colours::black.withAlpha(0.7f));
    g.strokePath(cablePath, juce::PathStrokeType(cableThickness + 2.0f,
        juce::PathStrokeType::curved));

    // Draw main cable
    g.setColour(cableColour);
    g.strokePath(cablePath, juce::PathStrokeType(cableThickness,
        juce::PathStrokeType::curved));

    // DEBUG: Draw control points (remove in production)
    /*
    g.setColour(juce::Colours::red.withAlpha(0.5f));
    g.fillEllipse(controlPoint.x - 3, controlPoint.y - 3, 6, 6);
    */
}

void MixerView::paintRoutingCables(juce::Graphics& g)
{
	auto channelUuid = ChannelRouteViewSingleton::getInstance().getChannelUuid();
	juce::ValueTree channelInfo = projectData.getChildWithName(channelUuid);
	juce::ValueTree channelsRoutedTo = channelInfo.getChildWithName("channelsRoutedTo");
    auto cableColour = juce::Colour::fromString(channelInfo.getProperty("Colour").toString());
	auto routeToPoint = getChannelRouteToButtonCenter(channelUuid);
    DBG("----------------------------------------");
	DBG("RouteToPoint: " << routeToPoint.toString());

    for (int i = 0; i < channelsRoutedTo.getNumChildren(); ++i)
    {
		auto routedChannelUuid = channelsRoutedTo.getChild(i).getType().toString();
		auto routeInPoint = getChannelRouteInButtonCenter(routedChannelUuid);
		DBG("RouteInPoint: " << routeInPoint.toString());
		// Draw the catenary cable
		drawCatenaryCable(g, routeToPoint.toFloat(), routeInPoint.toFloat(), 4.0f, cableColour, 1.0f, 50);
    }

	const auto state = RoutingActionStateManager::getInstance().getCurrentState();
    switch (state)
    {
    case RoutingActionStateManager::RoutingState::ROUTING_ON:
        if (cableOverlay)
        {
			juce::Point<int> screenMousePos = juce::Desktop::getInstance().getMousePosition();
            auto localPoint = getLocalPoint(nullptr, screenMousePos);
			auto mousePoint = juce::Point<float>(juce::jmax(localPoint.x, 0), juce::jmax(localPoint.y, 0));
            drawCatenaryCable(g, routeToPoint.toFloat(), mousePoint.toFloat(), 4.0f, cableColour, 1.0f, 50);
        }
        break;
    default:
        break;
    }
}
