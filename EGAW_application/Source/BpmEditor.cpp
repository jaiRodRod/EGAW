/*
  ==============================================================================

    BpmEditor.cpp
    Created: 20 Feb 2025 11:52:05am
    Author:  jaezro

  ==============================================================================
*/

#include "BpmEditor.h"

BpmEditor::BpmEditor(juce::ValueTree& projectData) : projectData(projectData)
{
    projectData.addListener(this);  

    bpmEditor.setMultiLine(false, false);
    bpmEditor.setReturnKeyStartsNewLine(false);
    bpmEditor.setInputRestrictions(6, "0123456789.");
    bpmEditor.setText(projectData.getProperty("bpm").toString(), false); // Texto inicial
    bpmEditor.setJustification(juce::Justification::centredRight);
    bpmEditor.onReturnKey = [this] { bpmEditDone(); };
    bpmEditor.onFocusLost = [this] { bpmEditDone(); };
    bpmEditor.onEscapeKey = [this] { checkValidBpmValueAndSet("1000000"); };
    addAndMakeVisible(bpmEditor);

    lastBpmValue = projectData.getProperty("bpm").toString();

}

BpmEditor::~BpmEditor()
{
    projectData.removeListener(this);
}

void BpmEditor::valueTreePropertyChanged(juce::ValueTree& tree, const juce::Identifier& property)
{
    if (tree == projectData && property == juce::Identifier("bpm")) 
    {
        auto newText = tree[property].toString();

        // Actualiza la UI sin disparar onTextChange
        bpmEditor.setText(newText, false);
    }
}

void BpmEditor::resized()
{
    //
    juce::Font textFont((float)getLocalBounds().getHeight() * 0.8f);
    bpmEditor.setFont(textFont);
    if (!initialised)
    {
        bpmEditor.clear();
        bpmEditor.setText("90.00", false);
        //checkValidBpmValueAndSet("90.00");
        initialised = true;
    }
    bpmEditor.setBounds(getLocalBounds());
}

void BpmEditor::bpmEditDone()
{
    checkValidBpmValueAndSet(bpmEditor.getText());
}

void BpmEditor::checkValidBpmValueAndSet(juce::String bpmValue)
{
    static const std::regex regexPattern(R"(^\d{0,3}(\.\d{0,2})?$)");

    if (!std::regex_match(bpmValue.toStdString(), regexPattern))
    {
        // Si el texto no coincide con el formato, revertir al último valor válido
        
        //setText(lastBpmValue, false);
        //audioSystemBus.getProjectData()->setProperty("bpm", lastBpmValue, nullptr);
        bpmEditor.setText(lastBpmValue, false);
    }
    else
    {
        //DBG(bpmValue.substring(bpmValue.lastIndexOf(".")));

        // Forzar el formato XXX.XX
        if (!bpmValue.contains("."))
        {
            bpmValue += ".00";
        }
        if (bpmValue.startsWith("."))
        {
            bpmValue = "000" + bpmValue;
        }
        else if (bpmValue.endsWith("."))
        {
            bpmValue += "00";
        }
        else if (bpmValue.contains(".") && bpmValue.substring(bpmValue.lastIndexOf(".") + 1).length() == 1)
        {
            bpmValue += "0";
        }
        else if (bpmValue.getLastCharacter() == '0' && bpmValue.length() == 5)
        {
            bpmValue = bpmValue.substring(0, 4) + "0";
        }

        lastBpmValue = bpmValue;
        projectData.setProperty("bpm", lastBpmValue, nullptr);
    }
}
