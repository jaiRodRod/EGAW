/*
  ==============================================================================

    UserInterfaceManager.cpp
    Created: 19 Feb 2025 8:58:10am
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "UserInterfaceManager.h"


//==============================================================================
UserInterfaceManager::UserInterfaceManager(juce::ValueTree& projectData) 
    : menuBarModel()
    , menuBarComponent(&menuBarModel)
    , projectData(projectData)
    , footer(projectData)
    , selectedView(2)
    , mixerView(projectData)
    , playlistView(projectData)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    SignalManagerUI::getInstance()->addListener(this);
    RoutingActionStateManager::getInstance()->addListener(this);
    projectData.addListener(this);

    getLookAndFeel().setColour(juce::ResizableWindow::backgroundColourId, juce::Colour(50,50,50));
    getLookAndFeel().setColour(juce::PopupMenu::backgroundColourId, juce::Colours::grey);
    getLookAndFeel().setColour(MENU_BACKGROUND_COLOUR_ID, juce::Colours::grey);
    //Para cambiar cosas del MenuBarComponent hay que construir una clase que herede de esta y modificar el paint
    //menuBarComponent.setLookAndFeel(&getLookAndFeel());

    addAndMakeVisible (menuBarComponent);
    addAndMakeVisible (footer);
    addAndMakeVisible (mixerView);   
    addAndMakeVisible (playlistView);   
}

UserInterfaceManager::~UserInterfaceManager()
{
    SignalManagerUI::getInstance()->removeListener(this);
    RoutingActionStateManager::getInstance()->removeListener(this);
    projectData.removeListener(this);
}

void UserInterfaceManager::valueChanged(juce::Value& value)
{
    auto signal = SignalManagerUI::getInstance()->getCurrentSignal();

    switch (signal)
    {
    case SignalManagerUI::Signal::REBUILD_UI:
        rebuildUI();
        break;
    default:
        break;
    }

    auto state = RoutingActionStateManager::getInstance()->getCurrentState();
    switch (state)
    {
    case RoutingActionStateManager::RoutingState::ROUTING_ON:
        invokeRoutingOverlay();
        break;
    case RoutingActionStateManager::RoutingState::ROUTING_OFF:
        removeRoutingOverlay();
        break;
    case RoutingActionStateManager::RoutingState::REMOVING_ROUTE:
        invokeRoutingOverlay();
        break;
    default:
        break;
    }
}

void UserInterfaceManager::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
    if (projectData == treeWhosePropertyHasChanged)
    {
        if (property == juce::Identifier("View"))
        {
            if (projectData.getProperty(property) == "MIXER")
            {
                selectedView = 0;
            }
            else if (projectData.getProperty(property) == "PLAYLIST")
            {
                selectedView = 1;
            }
            else if (projectData.getProperty(property) == "ROOM")
            {
                selectedView = 2;
            }
            resized();
        }
    }
}

void UserInterfaceManager::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
}

void UserInterfaceManager::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    auto area = getLocalBounds();
    
    auto displayBounds = DisplaySingleton::getInstance()->getMainDisplayBounds();

    auto menuBarHeight = (int) getLookAndFeel().getPopupMenuFont().getHeight() + 6;
    menuBarComponent.setBounds (area.removeFromTop (menuBarHeight));

    auto footerHeight = displayBounds.getHeight() / 24 + area.getHeight() / 24;
    footer.setBounds (area.removeFromBottom (footerHeight));

    if (selectedView == 0)
    {
        mixerView.setBounds (area);
        playlistView.setBounds (0, 0, 0, 0);
    }
    else if (selectedView == 1)
    {
        mixerView.setBounds (0,0,0,0); //Check si es mas costoso con el tiempo minimizar los componentes o reconstruirlos
        playlistView.setBounds (area);

        DBG("Playlist View on Display");
    }
    else if (selectedView == 2)
    {
        mixerView.setBounds (0, 0, 0, 0);
        playlistView.setBounds (0, 0, 0, 0);

        DBG("Listening Room View on Display");
    }

}

void UserInterfaceManager::rebuildUI()
{
    /*
    removeAllChildren();

    addAndMakeVisible (menuBarComponent);
    addAndMakeVisible (footer);
    addAndMakeVisible (mixerView);
    addAndMakeVisible (playlistView);
    */
}

void UserInterfaceManager::invokeRoutingOverlay()
{
    //Activate moving scrollbar
    //Easier to do with the draggable component on
    routingOverlay.setBounds(getLocalBounds());
    addAndMakeVisible(routingOverlay);
}

void UserInterfaceManager::removeRoutingOverlay()
{
    removeChildComponent(&routingOverlay);
}
