/*
  ==============================================================================

    RoutingActionStateManager.h
    Created: 24 Feb 2025 5:56:44pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class RoutingActionStateManager : public juce::Value::Listener
{
public:

    enum class RoutingState
    {
        ROUTING_OFF,
        ROUTING_ON,
        COMPLETED_ROUTING,
        REMOVING_ROUTE,
        COMPLETED_REMOVING_ROUTE,
        VIEWING_ROUTES
    };

    //Routing off es cuando no se usa nada de routing
    //Routing on es cuando se va a enrutar un canal y se hace con click izquierdo sobre la clavija
    //Removing route es cuando se va a desentutar un canal y se hace con click derecho sobre la clavija
    //Viewing routes se hace para ver a que canales se enruta el canal con el simbolo del ojito sobre la clavija, se hace sobre el originChannel

    RoutingState getCurrentState() const;
    void setState(RoutingState newState);
    void addListener(juce::Value::Listener* listener);
    void removeListener(juce::Value::Listener* listener);
    juce::Value getValue() { return state; };

    juce::String getOriginChannelUuid();
    void setOriginChannelUuid(juce::String originChannelUuid);
    juce::String getDestinyChannelUuid();
    void setDestinyChannelUuid(juce::String destinyChannelUuid);

    void routingOff();

    JUCE_DECLARE_SINGLETON(RoutingActionStateManager, true);

private:

    RoutingActionStateManager();
    ~RoutingActionStateManager() { clearSingletonInstance(); };

    void valueChanged(juce::Value& value) override
    {
        //Se declara para deshacer la abstraccion pero no se utilizará nada
    }

    juce::Value state;
    juce::String originChannelUuid;
    juce::String destinyChannelUuid;
};