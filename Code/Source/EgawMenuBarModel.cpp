/*
  ==============================================================================

    EgawMenuBarModel.cpp
    Created: 19 Feb 2025 11:23:10am
    Author:  jaezro

  ==============================================================================
*/

#include "EgawMenuBarModel.h"

EgawMenuBarModel::EgawMenuBarModel() : MenuBarModel()
{
    menuBarNames.add("File");

    juce::PopupMenu::Item load("Load");
    load.setAction([this] { fileLoadAction(); });

    juce::PopupMenu::Item save("Save");
    save.setAction([this] { fileSaveAction(); });

    juce::PopupMenu::Item saveAs("Save as...");
    saveAs.setAction([this] { fileSaveAsAction(); });

    juce::PopupMenu::Item render("Render");
    render.setAction([this] { fileRenderAction(); });

    fileMenu.addItem(load);
    fileMenu.addItem(save);
    fileMenu.addItem(saveAs);
    fileMenu.addItem(render);
}


juce::StringArray EgawMenuBarModel::getMenuBarNames()
{
    return menuBarNames;
}

juce::PopupMenu EgawMenuBarModel::getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName)
{
    return fileMenu;
}

void EgawMenuBarModel::menuItemSelected(int menuItemID, int ropLevelMenuIndex)
{

}

void EgawMenuBarModel::fileLoadAction()
{
    SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::LOAD_FILE);
}

void EgawMenuBarModel::fileSaveAction()
{
    SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::SAVE_FILE);
}

void EgawMenuBarModel::fileSaveAsAction()
{
    SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::SAVE_AS_FILE);
}

void EgawMenuBarModel::fileRenderAction()
{
    SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::RUN_EXPORT_PROCESS);
}
