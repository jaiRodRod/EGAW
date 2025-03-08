/*
  ==============================================================================

    UndoManager.h
    Created: 12 Feb 2025 5:50:41pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

//Hacer singleton
class UndoManager : public juce::UndoManager
{
public:
    JUCE_DECLARE_SINGLETON(UndoManager, false);

    // Invocar mediante el singleton la instancia existente del UndoManager
    //  UndoManager& undoManager = UndoManager::getInstance();

    // Usar el undoManager como se haría con un juce::UndoManager normal
    // undoManager.perform(new MyUndoableAction());

private:

    UndoManager() = default;
    ~UndoManager() = default;

    UndoManager(const UndoManager&) = delete;
    UndoManager& operator=(const UndoManager&) = delete;
};