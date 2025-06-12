/*
  ==============================================================================

    EgawMenuBarModel.h
    Created: 19 Feb 2025 11:23:10am
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SignalManagerUI.h"

#define FILE_MENU 0
#define FILE_MENU_NAME "File"

//REFACTORIZAR PARA USAR UN BINARIO INTERNO CON DATOS

class EgawMenuBarModel : public juce::MenuBarModel
{
public:

    EgawMenuBarModel();

    juce::StringArray getMenuBarNames() override;
    juce::PopupMenu getMenuForIndex(int, const juce::String&) override;
    void menuItemSelected(int, int) override;

private:

    void fileLoadAction();
    void fileSaveAction();
    void fileSaveAsAction();
    void fileRenderAction();

    juce::StringArray menuBarNames;
    juce::PopupMenu fileMenu;
};
