/*
  ==============================================================================

    RoutingOverlay.cpp
    Created: 25 Feb 2025 5:14:07pm
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RoutingOverlay.h"
#include "RouteInButton.h"

//==============================================================================
RoutingOverlay::RoutingOverlay()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    //setAlwaysOnTop(true);
    setInterceptsMouseClicks(true, true);
    setBounds(getLocalBounds());

    //toBack();
}

RoutingOverlay::~RoutingOverlay()
{
}

void RoutingOverlay::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::transparentBlack);
}

void RoutingOverlay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void RoutingOverlay::mouseDown(const juce::MouseEvent& event)
{
    if (auto* parent = getParentComponent())
    {
        // Convert the click position from overlay coordinates to the parent's coordinate system.
        auto parentPoint = parent->getLocalPoint(this, event.getPosition());
        juce::Component* underlyingTextButton = nullptr;

        // Iterate through the parent's children in reverse order.
        for (int i = parent->getNumChildComponents() - 1; i >= 0; --i)
        {
            auto* child = parent->getChildComponent(i);
            if (child == this || !child->isVisible())
                continue;

            // Convert the parent's point into this child's coordinate system.
            //DBG("Child bounds: " << child->getLocalBounds().toString());
            auto childPoint = child->getLocalPoint(parent, parentPoint);
            auto childBounds = child->getLocalBounds();
            if (childBounds.contains(childPoint.getX(), childPoint.getY()))
            {
                // Use our recursive helper to try and find a TextButton.
                underlyingTextButton = recursiveFindRouteInButton(child, childPoint);
                if (underlyingTextButton != nullptr)
                    break;
            }
            //// Use our recursive helper to try and find a TextButton.
            //underlyingTextButton = recursiveFindRouteInButton(child, childPoint);
            //if (underlyingTextButton != nullptr)
            //    break;
        }

        if (underlyingTextButton != nullptr)
        {
            DBG("Button clicked.");
            // Process the button click if needed.
            //RoutingActionStateManager::getInstance()->setState(RoutingActionStateManager::RoutingState::ROUTING_OFF);
        }
        else
        {
            DBG("Clicked outside button.");
            RoutingActionStateManager::getInstance().setState(RoutingActionStateManager::RoutingState::ROUTING_OFF);
        }
    }
}


juce::Component* RoutingOverlay::recursiveFindRouteInButton(juce::Component* comp, juce::Point<int> pointInCompCoords)
{
    // First, if the point is not even within this component, return null.
    auto compBounds = comp->getLocalBounds();
    if (!compBounds.contains(pointInCompCoords.x, pointInCompCoords.y))
        return nullptr;

    // If this component is a RouteInButton, return it.
    if (dynamic_cast<RouteInButton*>(comp) != nullptr)
    {
        auto* routeInButton = dynamic_cast<RouteInButton*>(comp);
        if(routeInButton->isEnabled())
            routeInButton->triggerClick();
        return comp;
    }

    // Otherwise, iterate over its children (front-to-back) to see if any contains the click.
    for (int i = comp->getNumChildComponents() - 1; i >= 0; --i)
    {
        auto* child = comp->getChildComponent(i);
        if (!child->isVisible())
            continue;

        // Convert the point from the parent's coordinate space into the child's.
        auto childPoint = child->getLocalPoint(comp, pointInCompCoords);
        if (auto* foundButton = recursiveFindRouteInButton(child, childPoint))
            return foundButton;
    }

    // No button found in this branch.
    return nullptr;
}
