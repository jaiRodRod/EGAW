/*
  ==============================================================================

    RoutingActionStateManager.cpp
    Created: 24 Feb 2025 5:56:44pm
    Author:  jaezro

  ==============================================================================
*/

#include "RoutingActionStateManager.h"

RoutingActionStateManager& RoutingActionStateManager::getInstance()
{
    static RoutingActionStateManager instance;
    return instance;
}

RoutingActionStateManager::RoutingActionStateManager() = default;
RoutingActionStateManager::~RoutingActionStateManager() { cancelPendingUpdate(); }

// Thread-safe state management
RoutingActionStateManager::RoutingState RoutingActionStateManager::getCurrentState() const noexcept
{
    return currentState.load(std::memory_order_acquire);
}

void RoutingActionStateManager::setState(RoutingState newState)
{
    bool needsTrigger = false;
    {
        juce::ScopedLock lock(activeListenersLock);
        stateQueue.push_back(newState);
        needsTrigger = !notificationPending.exchange(true);
    }
    if (needsTrigger) triggerAsyncUpdate();
}

// Thread-safe listener management
void RoutingActionStateManager::addListener(juce::MessageListener* listener)
{
    juce::ScopedLock lock(activeListenersLock);
    activeListeners.insert(listener);
    listeners.add(listener);
}

void RoutingActionStateManager::removeListener(juce::MessageListener* listener)
{
    juce::ScopedLock lock(activeListenersLock);
    activeListeners.erase(listener);
    listeners.remove(listener);
}

// Thread-safe string access
juce::String RoutingActionStateManager::getOriginChannelUuid() const
{
    juce::ScopedLock lock(stringLock);
    return originChannelUuid;
}

void RoutingActionStateManager::setOriginChannelUuid(const juce::String& uuid)
{
    juce::ScopedLock lock(stringLock);
    originChannelUuid = uuid;
}

juce::String RoutingActionStateManager::getDestinyChannelUuid() const
{
    juce::ScopedLock lock(stringLock);
    return destinyChannelUuid;
}

void RoutingActionStateManager::setDestinyChannelUuid(const juce::String& uuid)
{
    juce::ScopedLock lock(stringLock);
    destinyChannelUuid = uuid;
}

void RoutingActionStateManager::routingOff()
{
    setState(RoutingState::ROUTING_OFF);
    setOriginChannelUuid({});
    setDestinyChannelUuid({});
}

// Core message delivery
void RoutingActionStateManager::handleAsyncUpdate()
{
    std::deque<RoutingState> localQueue;
    {
        juce::ScopedLock lock(activeListenersLock);
        localQueue.swap(stateQueue);
        notificationPending.store(false, std::memory_order_release);
    }

    for (const auto state : localQueue)
    {
        currentState.store(state, std::memory_order_release);

        juce::MessageManager::callAsync([this, state]() {
            RoutingMessage msg(static_cast<int>(state));

            listeners.call([this, &msg](juce::MessageListener& l) {
                juce::ScopedLock lock(activeListenersLock);
                if (activeListeners.find(&l) != activeListeners.end()) {
                    l.handleMessage(msg);
                }
                });
            });
    }
}