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
TransportBoxComponent::TransportBoxComponent(juce::ValueTree& projectData, juce::ValueTree& playheadState) 
    : projectData(projectData)
    , playheadState(playheadState)
    , transportComponent()
{
    transportViewPosition = 0;
    addAndMakeVisible(transportComponent);

    playheadState.addListener(this);
}

TransportBoxComponent::~TransportBoxComponent()
{
    playheadState.removeListener(this);
}

void TransportBoxComponent::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
    if (treeWhosePropertyHasChanged == playheadState)
    {
        if (property.toString() == "position")
        {
            auto totalTime = (double)playheadState.getProperty("time");
            auto blockPosition = (double)playheadState.getProperty("position");
            auto sampleRate = (double)playheadState.getProperty("sampleRate");
            juce::RelativeTime time(blockPosition / sampleRate);

            transportViewPosition = (double)projectData.getProperty("Zoom") * time.inSeconds();

            // Check if necessary
            // SignalManagerUI::getInstance()->setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);

            //Replaced by this�?
            auto transportWidth = 20 + 20; //El tama�o del transport y control de hitbox
            transportComponent.setBounds(transportViewPosition - (transportWidth / 2), 0, transportWidth, getLocalBounds().getHeight());
        }
    }
}

void TransportBoxComponent::mouseDoubleClick(const juce::MouseEvent& event)
{
    /*
    auto clickPoint = event.getMouseDownPosition().getX();
    auto clickPosInSeconds = (double)clickPoint / (double)projectData.getProperty("Zoom");
    auto samplePos = clickPosInSeconds * GlobalPlayhead::getInstance()->getSampleRate();

    GlobalPlayhead::getInstance()->setPlayheadPosition(samplePos);
    */
}

void TransportBoxComponent::mouseDrag(const juce::MouseEvent& event)
{
    /*
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
    */

    /*
    * QUITADO ANTES DEL REFACTORRR
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

    auto transportWidth = 20 + 20; //El tama�o del transport y control de hitbox
    //transportComponent.setTopLeftPosition(transportViewPosition - (transportWidth/2), 0);
    //transportComponent.setSize(transportWidth,area.getHeight());
    transportComponent.setBounds(transportViewPosition - (transportWidth / 2), 0, transportWidth, area.getHeight());
}
