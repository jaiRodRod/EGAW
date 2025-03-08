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
UserInterfaceManager::UserInterfaceManager(juce::ValueTree& projectData) : menuBarModel(), menuBarComponent(&menuBarModel)
, projectData(projectData), footer(projectData), mixerView(projectData)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    SignalManagerUI::getInstance()->addListener(this);
    RoutingActionStateManager::getInstance()->addListener(this);

    getLookAndFeel().setColour(juce::ResizableWindow::backgroundColourId, juce::Colour(50,50,50));
    getLookAndFeel().setColour(juce::PopupMenu::backgroundColourId, juce::Colours::grey);
    getLookAndFeel().setColour(MENU_BACKGROUND_COLOUR_ID, juce::Colours::grey);
    //Para cambiar cosas del MenuBarComponent hay que construir una clase que herede de esta y modificar el paint
    //menuBarComponent.setLookAndFeel(&getLookAndFeel());

    addAndMakeVisible (menuBarComponent);
    addAndMakeVisible (footer);
    addAndMakeVisible (mixerView);   
}

UserInterfaceManager::~UserInterfaceManager()
{
    SignalManagerUI::getInstance()->removeListener(this);
    RoutingActionStateManager::getInstance()->removeListener(this);
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

    mixerView.setBounds (area);

}

void UserInterfaceManager::rebuildUI()
{
    removeAllChildren();

    addAndMakeVisible (menuBarComponent);
    addAndMakeVisible (footer);
    addAndMakeVisible (mixerView);
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
