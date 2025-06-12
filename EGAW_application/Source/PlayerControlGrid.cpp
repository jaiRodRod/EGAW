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
PlayerControlGrid::PlayerControlGrid(juce::ValueTree& projectData, juce::ValueTree& playheadState) : projectData(projectData), playheadState(playheadState)/*, bpmEditor(projectData)*/
{
    playheadState.addListener(this);

    playing = false;

    //playButton.setButtonText("Play");
    playButton.setToggleState(false, false);
    playButton.onClick = [this] { playButtonClicked(); };
    addAndMakeVisible(playButton);

    stopButton.setToggleState(false, false);
    stopButton.onClick = [this] { stopButtonClicked(); };
    addAndMakeVisible(stopButton);

    /*addAndMakeVisible(bpmEditor);*/

}

PlayerControlGrid::~PlayerControlGrid()
{
    playheadState.removeListener(this);
}

void PlayerControlGrid::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
    if (treeWhosePropertyHasChanged == playheadState)
    {
        if (property.toString() == "isPlaying")
        {
            if ((bool)playheadState.getProperty("isPlaying") == true)
            {
                setPlaying(true);
                playButton.setButtonText("Pause");
            }
            else
            {
                setPlaying(false);
                playButton.setButtonText("Play");
            }
        }
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
    playerGrid.templateColumns = { juce::Grid::Fr(1), juce::Grid::Fr(1)}; // Dos columnas de igual tama    playerGrid.templateRows = { juce::Grid::Fr(1) }; // Una fila

    playerGrid.items.add(juce::GridItem (playButton).withHeight((float) gridBounds.getHeight()));
    playerGrid.items.add(juce::GridItem (stopButton).withHeight((float) gridBounds.getHeight()));
    /*playerGrid.items.add(juce::GridItem (bpmEditor).withHeight((float)gridBounds.getHeight()));*/

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
        playButton.setToggleState(true, false);
		playButton.repaint();
        SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::PLAY_AUDIO);
    }
    else if (playing == true)
    {
        playButton.setToggleState(false, false);
		playButton.repaint();
        SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::PAUSE_AUDIO);
    }
}

void PlayerControlGrid::stopButtonClicked()
{
    playButton.setToggleState(false, false);
    playButton.repaint();
    SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::STOP_AUDIO);
}