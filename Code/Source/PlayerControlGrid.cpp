/*
  ==============================================================================

    PlayerControlGrid.cpp
    Created: 19 Feb 2025 4:26:12pm
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlayerControlGrid.h"

//==============================================================================
PlayerControlGrid::PlayerControlGrid(juce::ValueTree& projectData) : projectData(projectData), bpmEditor(projectData)
{
    SignalManagerUI::getInstance()->addListener(this);

    playing = false;

    playButton.setButtonText("Play");
    playButton.onClick = [this] { playButtonClicked(); };
    addAndMakeVisible(playButton);

    stopButton.setButtonText("Stop");
    stopButton.onClick = [this] { stopButtonClicked(); };
    addAndMakeVisible(stopButton);

    addAndMakeVisible(bpmEditor);

}

PlayerControlGrid::~PlayerControlGrid()
{

}

void PlayerControlGrid::valueChanged(juce::Value& value)
{
    auto signal = SignalManagerUI::getInstance()->getCurrentSignal();

    switch (signal)
    {
    case SignalManagerUI::Signal::PLAY_AUDIO:
        setPlaying(true);
        playButton.setButtonText("Pause");
        break;
    case SignalManagerUI::Signal::PAUSE_AUDIO:
        setPlaying(false);
        playButton.setButtonText("Play");
        break;
    case SignalManagerUI::Signal::STOP_AUDIO:
        setPlaying(false);
        playButton.setButtonText("Play");
        break;
    default:
        break;
    }
}

void PlayerControlGrid::paint (juce::Graphics& g)
{
    //g.fillAll (juce::Colours::red);   // clear the background
}

void PlayerControlGrid::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    auto gridBounds = getLocalBounds();
    //DBG("Grid bounds: " << gridBounds.toString());

    playerGrid.items.clear();

    // Definir el layout del Grid
    playerGrid.templateColumns = { juce::Grid::Fr(1), juce::Grid::Fr(1), juce::Grid::Fr(2)}; // Dos columnas de igual tama    playerGrid.templateRows = { juce::Grid::Fr(1) }; // Una fila

    playerGrid.items.add(juce::GridItem (playButton).withHeight((float) gridBounds.getHeight()));
    playerGrid.items.add(juce::GridItem (stopButton).withHeight((float) gridBounds.getHeight()));
    playerGrid.items.add(juce::GridItem (bpmEditor).withHeight((float) gridBounds.getHeight()));

    playerGrid.performLayout(gridBounds);
}

void PlayerControlGrid::setPlaying(bool value)
{
    playing = value;
}

void PlayerControlGrid::playButtonClicked()
{
    if (playing == false)
    {
        SignalManagerUI::getInstance()->setSignal(SignalManagerUI::Signal::PLAY_AUDIO);
    }
    else if (playing == true)
    {
        SignalManagerUI::getInstance()->setSignal(SignalManagerUI::Signal::PAUSE_AUDIO);
    }
}

void PlayerControlGrid::stopButtonClicked()
{
    SignalManagerUI::getInstance()->setSignal(SignalManagerUI::Signal::STOP_AUDIO);
}