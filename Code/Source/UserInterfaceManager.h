/*
  ==============================================================================

    UserInterfaceManager.h
    Created: 19 Feb 2025 8:58:10am
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "DisplaySingleton.h"
#include "Footer.h"
#include "EgawMenuBarModel.h"
#include "MixerView.h"
#include "SignalManagerUI.h"
#include "RoutingActionStateManager.h"
#include "RoutingOverlay.h"

#define MENU_BACKGROUND_COLOUR_ID 0

//==============================================================================
/*
*/
class UserInterfaceManager  : public juce::Component, public juce::Value::Listener
{
public:
    UserInterfaceManager(juce::ValueTree&);
    ~UserInterfaceManager() override;

    void valueChanged(juce::Value& value) override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    void rebuildUI();

    void invokeRoutingOverlay();
    void removeRoutingOverlay();

    juce::ValueTree projectData;
    EgawMenuBarModel menuBarModel;
    juce::MenuBarComponent menuBarComponent;
    Footer footer;

    MixerView mixerView;

    RoutingOverlay routingOverlay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UserInterfaceManager)
};
