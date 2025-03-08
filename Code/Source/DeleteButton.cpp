/*
  ==============================================================================

    DeleteButton.cpp
    Created: 6 Mar 2025 9:50:11am
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeleteButton.h"

//==============================================================================
DeleteButton::DeleteButton(juce::ValueTree& projectData, juce::String channelUuid) : juce::TextButton(), projectData(projectData), channelUuid(channelUuid)
{
    setButtonText("Delete channel");
    onClick = [this] {showAcceptPopUp();};
}

void DeleteButton::showAcceptPopUp()
{
    bool result = juce::AlertWindow::showOkCancelBox(
        juce::AlertWindow::QuestionIcon,
        "Eliminar canal",
        "¿Estas seguro que quieres eliminar el canal?",
        "Si",
        "No",
        nullptr,
        juce::ModalCallbackFunction::create([this](int result)
            {
                if (result == 1)
                {
                    // Handle deletion here
                    deleteChannel();
                }
            })
    );
}

void DeleteButton::deleteChannel()
{
    DBG("DELETING CHANNEL");
    projectData.getChildWithName("channelOrder").removeChild(
        projectData.getChildWithName("channelOrder").getChildWithName(channelUuid)
        , nullptr
    );
    projectData.removeChild(projectData.getChildWithName(channelUuid), nullptr);
}


/*
DeleteButton::~DeleteButton()
{
}

void DeleteButton::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("DeleteButton", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void DeleteButton::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
*/