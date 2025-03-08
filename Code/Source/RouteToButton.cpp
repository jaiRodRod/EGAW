/*
  ==============================================================================

    RouteToButton.cpp
    Created: 27 Feb 2025 11:35:17am
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RouteToButton.h"

//==============================================================================
RouteToButton::RouteToButton(juce::String channelUuid) : juce::TextButton(), channelUuid(channelUuid)
{
    onClick = [this] {doRouteTo();};
    onRightClick = [this] { doRemoveRouteTo(); };
}

void RouteToButton::mouseUp(const juce::MouseEvent& event)
{
    if (event.mods.isRightButtonDown())
    {
        if (isEnabled() && contains(event.getPosition()))
            if (onRightClick)
                onRightClick();
    }
    else
    {
        TextButton::mouseUp(event);
    }
}

void RouteToButton::doRouteTo()
{
    RoutingActionStateManager::getInstance()->setOriginChannelUuid(channelUuid);
    RoutingActionStateManager::getInstance()->setState(RoutingActionStateManager::RoutingState::ROUTING_ON);
}

void RouteToButton::doRemoveRouteTo()
{
    RoutingActionStateManager::getInstance()->setOriginChannelUuid(channelUuid);
    RoutingActionStateManager::getInstance()->setState(RoutingActionStateManager::RoutingState::REMOVING_ROUTE);
}

/*
RouteToButton::~RouteToButton()
{
}

void RouteToButton::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("RouteToButton", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void RouteToButton::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
*/