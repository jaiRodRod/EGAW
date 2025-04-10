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
#include "PlaylistView.h";
#include "SignalManagerUI.h"
#include "RoutingActionStateManager.h"
#include "RoutingOverlay.h"

#define MENU_BACKGROUND_COLOUR_ID 0

//==============================================================================

class UserInterfaceManager : public juce::Component, public juce::ValueTree::Listener, public juce::MessageListener
{
public:
    UserInterfaceManager(juce::ValueTree&, juce::ValueTree&, GlobalPlayhead&);
    ~UserInterfaceManager() override;

    void valueTreePropertyChanged (juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override;
	void handleMessage(const juce::Message& message) override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    void rebuildUI();

    void invokeRoutingOverlay();
    void removeRoutingOverlay();

    juce::ValueTree projectData;
    juce::ValueTree playheadState;

    EgawMenuBarModel menuBarModel;
    juce::MenuBarComponent menuBarComponent;
    Footer footer;

    //0 = MIXER
    //1 = PLAYLIST
    //2 = ROOM
    int selectedView;
    MixerView mixerView;
    PlaylistView playlistView;

    RoutingOverlay routingOverlay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UserInterfaceManager)
};
