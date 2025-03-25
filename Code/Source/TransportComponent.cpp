/*
  ==============================================================================

    TransportComponent.cpp
    Created: 22 Mar 2025 12:14:48pm
    Author:  admin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TransportComponent.h"

//==============================================================================
TransportComponent::TransportComponent()
{
}

TransportComponent::~TransportComponent()
{
}

void TransportComponent::mouseDrag(const juce::MouseEvent& event)
{
    DBG("Transport click tunneling.");
    getParentComponent()->mouseDrag(event.getEventRelativeTo(getParentComponent()));
}

void TransportComponent::paint (juce::Graphics& g)
{
    //float lineX = getWidth() / 2.0f;
    //float topY = 0.0f;
    //float bottomY = viewportHeight;
    // g.setColour(juce::Colours::black);
    // g.drawLine(lineX, topY, lineX, bottomY, 5.0f);

    // Draw inverted triangle at the bottom of the component
    const float triangleSize = 10.0f;
    const float centerX = getWidth() / 2.0f;
    const float bottomY = getHeight();

    juce::Path triangle;
    triangle.addTriangle(
        centerX - triangleSize, bottomY - triangleSize,
        centerX + triangleSize, bottomY - triangleSize,
        centerX, bottomY
    );
    g.setColour(juce::Colours::black);
    g.fillAll(juce::Colours::aqua);
    g.fillPath(triangle);
}

void TransportComponent::resized()
{
    auto area = getLocalBounds();
}
