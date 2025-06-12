/*
  ==============================================================================

    TransportBoxComponent.cpp
    Created: 22 Mar 2025 11:36:09am
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TransportBoxComponent.h"

//==============================================================================
TransportBoxComponent::TransportBoxComponent(juce::ValueTree& projectData, juce::ValueTree& playheadState, GlobalPlayhead& globalPlayhead) 
    : projectData(projectData)
    , playheadState(playheadState)
	, globalPlayhead(globalPlayhead)
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

            //Replaced by this¿?
            auto transportWidth = 20 + 20; //El tamaño del transport y control de hitbox
            transportComponent.setBounds(transportViewPosition - (transportWidth / 2), 0, transportWidth, getLocalBounds().getHeight());
        }
    }
}

void TransportBoxComponent::mouseDoubleClick(const juce::MouseEvent& event)
{
    auto clickPoint = event.getMouseDownPosition().getX();
    auto clickPosInSeconds = (double)clickPoint / (double)projectData.getProperty("Zoom");
    auto sampleRate = (double)playheadState.getProperty("sampleRate");
    auto samplePos = clickPosInSeconds * sampleRate;

    globalPlayhead.setPlayheadPosition(samplePos);
}

void TransportBoxComponent::mouseDrag(const juce::MouseEvent& event)
{
    auto dragStartPoint = event.getMouseDownPosition().getX();
    auto dragStartPosInSeconds = (double)dragStartPoint / (double)projectData.getProperty("Zoom");
    auto sampleRate = (double)playheadState.getProperty("sampleRate");
    auto playheadPos = dragStartPosInSeconds * sampleRate;

    auto draggedDistanceX = event.getDistanceFromDragStartX();
    auto distanceInSeconds = (double)draggedDistanceX / (double)projectData.getProperty("Zoom");
    auto sampleDistance = distanceInSeconds * sampleRate;

    if (playheadPos + sampleDistance >= 0)
    {
        globalPlayhead.setPlayheadPosition(playheadPos + sampleDistance);
    }
    else
    {
        globalPlayhead.setPlayheadPosition(0);
    }
 }

void TransportBoxComponent::paint (juce::Graphics& g)
{
    g.fillAll (colourConstants::light_green.brighter(0.5f));   // clear the background
}

void TransportBoxComponent::resized()
{
    auto area = getLocalBounds();

    //setBounds(area);

    auto transportWidth = 20 + 20; //El tamaño del transport y control de hitbox
    //transportComponent.setTopLeftPosition(transportViewPosition - (transportWidth/2), 0);
    //transportComponent.setSize(transportWidth,area.getHeight());
    transportComponent.setBounds(transportViewPosition - (transportWidth / 2), 0, transportWidth, area.getHeight());
}
