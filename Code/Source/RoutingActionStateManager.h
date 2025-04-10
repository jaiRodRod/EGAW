/*
  ==============================================================================

    RoutingActionStateManager.h
    Created: 24 Feb 2025 5:56:44pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <unordered_set>

class RoutingMessage : public juce::Message
{
public:
    explicit RoutingMessage(int state) : routingState(state) {}
    const int routingState;
};

class RoutingActionStateManager : public juce::AsyncUpdater
{
public:
    enum class RoutingState {
        ROUTING_OFF,
        ROUTING_ON,
        COMPLETED_ROUTING,
        REMOVING_ROUTE,
        COMPLETED_REMOVING_ROUTE,
        VIEWING_ROUTES
    };

    static RoutingActionStateManager& getInstance();

    // Thread-safe state access
    RoutingState getCurrentState() const noexcept;
    void setState(RoutingState newState);

    // Thread-safe listener management
    void addListener(juce::MessageListener* listener);
    void removeListener(juce::MessageListener* listener);

    // Thread-safe string access
    juce::String getOriginChannelUuid() const;
    void setOriginChannelUuid(const juce::String& uuid);
    juce::String getDestinyChannelUuid() const;
    void setDestinyChannelUuid(const juce::String& uuid);

    void routingOff();

private:
    RoutingActionStateManager();
    ~RoutingActionStateManager() override;

    void handleAsyncUpdate() override;

    // State management
    std::atomic<RoutingState> currentState{ RoutingState::ROUTING_OFF };
    std::deque<RoutingState> stateQueue;
    std::atomic<bool> notificationPending{ false };

    // String storage
    juce::CriticalSection stringLock;
    juce::String originChannelUuid;
    juce::String destinyChannelUuid;

    // Listener management
    juce::ThreadSafeListenerList<juce::MessageListener> listeners;
    std::unordered_set<juce::MessageListener*> activeListeners;
    juce::CriticalSection activeListenersLock;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RoutingActionStateManager)
};