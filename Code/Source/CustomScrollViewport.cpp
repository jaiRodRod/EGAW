/*
  ==============================================================================

    CustomScrollViewport.cpp
    Created: 15 Mar 2025 7:03:26pm
    Author:  admin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CustomScrollViewport.h"

CustomScrollViewport::CustomScrollViewport(juce::Viewport& parentViewport) : parentViewport(parentViewport)
{
}

void CustomScrollViewport::mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel)
{
    /*
    if (std::abs(wheel.deltaY) > std::abs(wheel.deltaX))
    {
        juce::MouseEvent e(event.source, event.position, event.mods, event.pressure, event.orientation, event.rotation, 
                           event.tiltX, event.tiltY, &parentViewport, event.originalComponent, event.eventTime, event.mouseDownPosition, 
                           event.mouseDownTime, event.getNumberOfClicks(), event.mouseWasDraggedSinceMouseDown());
        parentViewport.mouseWheelMove(e, wheel);
        return;
    }

    Viewport::mouseWheelMove(event, wheel);
    */

    if (wheel.deltaY != 0.0f) // Only forward vertical scroll events
    {
        parentViewport.mouseWheelMove(event, wheel);
    }
    else
    {
        Viewport::mouseWheelMove(event, wheel);
    }
}
