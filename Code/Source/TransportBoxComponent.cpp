/*
  ==============================================================================

    TransportBoxComponent.cpp
    Created: 22 Mar 2025 11:36:09am
    Author:  admin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TransportBoxComponent.h"

//==============================================================================
TransportBoxComponent::TransportBoxComponent(juce::ValueTree& projectData) 
    : projectData(projectData)
    , transportComponent()
{
    transportViewPosition = 0;
    addAndMakeVisible(transportComponent);

    startTimerHz(30);
}

TransportBoxComponent::~TransportBoxComponent()
{
    stopTimer();
}

void TransportBoxComponent::timerCallback()
{
    const juce::ScopedLock lock(SharedLock_UI_Timers::lock);

    auto totalTime = GlobalPlayhead::getInstance()->getTimeLengthSeconds();
    auto blockPosition = GlobalPlayhead::getInstance()->getPlayheadPosition();
    auto sampleRate = GlobalPlayhead::getInstance()->getSampleRate();
    juce::RelativeTime time(blockPosition / sampleRate);

    transportViewPosition = (double)projectData.getProperty("Zoom") * time.inSeconds();

    resized();
}

void TransportBoxComponent::mouseDoubleClick(const juce::MouseEvent& event)
{
    auto clickPoint = event.getMouseDownPosition().getX();
    auto clickPosInSeconds = (double)clickPoint / (double)projectData.getProperty("Zoom");
    auto samplePos = clickPosInSeconds * GlobalPlayhead::getInstance()->getSampleRate();

    GlobalPlayhead::getInstance()->setPlayheadPosition(samplePos);
}

void TransportBoxComponent::mouseDrag(const juce::MouseEvent& event)
{
    auto dragStartPoint = event.getMouseDownPosition().getX();
    auto dragStartPosInSeconds = (double)dragStartPoint / (double)projectData.getProperty("Zoom");
    auto playheadPos = dragStartPosInSeconds * GlobalPlayhead::getInstance()->getSampleRate();
    //DBG("Drag start point: " << dragStartPoint.getX());
    //DBG("Trnasport component top left coord X: " << transportComponent.getBounds().getTopLeft().getX());
    auto draggedDistanceX = event.getDistanceFromDragStartX();
    auto distanceInSeconds = (double)draggedDistanceX / (double)projectData.getProperty("Zoom");
    DBG("Distance in seconds");
    auto sampleDistance = distanceInSeconds * GlobalPlayhead::getInstance()->getSampleRate();

    GlobalPlayhead::getInstance()->setPlayheadPosition(playheadPos + sampleDistance);
    /*
    if (transportComponent.getBounds().contains(dragStartPoint))
    {
        auto draggedDistanceX = event.getDistanceFromDragStartX();
        auto distanceInSeconds = (double)draggedDistanceX / 50.0f;
        auto sampleDistance = distanceInSeconds * GlobalPlayhead::getInstance()->getSampleRate();

        auto playheadPos = GlobalPlayhead::getInstance()->getPlayheadPosition();
        GlobalPlayhead::getInstance()->setPlayheadPosition(playheadPos + sampleDistance);
    }
    */
 }

void TransportBoxComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::purple);   // clear the background
}

void TransportBoxComponent::resized()
{
    auto area = getLocalBounds();

    auto transportWidth = 20 + 20; //El tamaño del transport y control de hitbox
    //transportComponent.setTopLeftPosition(transportViewPosition - (transportWidth/2), 0);
    //transportComponent.setSize(transportWidth,area.getHeight());
    transportComponent.setBounds(transportViewPosition - (transportWidth / 2), 0, transportWidth, area.getHeight());
}
