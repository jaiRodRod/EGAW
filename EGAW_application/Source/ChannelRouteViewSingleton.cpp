/*
  ==============================================================================

    ChannelRouteViewSingleton.cpp
    Created: 23 May 2025 5:07:36pm
    Author:  jaezro

  ==============================================================================
*/

#include "ChannelRouteViewSingleton.h"

ChannelRouteViewSingleton& ChannelRouteViewSingleton::getInstance()
{
    static ChannelRouteViewSingleton instance;
    return instance;
}

ChannelRouteViewSingleton::ChannelRouteViewSingleton() = default;
ChannelRouteViewSingleton::~ChannelRouteViewSingleton() { cancelPendingUpdate(); }

ChannelRouteViewSingleton::RouteViewState ChannelRouteViewSingleton::getCurrentState() const noexcept
{
    return currentState.load(std::memory_order_acquire);
}

void ChannelRouteViewSingleton::setState(RouteViewState newState)
{
    bool needsTrigger = false;
    {
        juce::ScopedLock lock(activeListenersLock);
        stateQueue.push_back(newState);
        needsTrigger = !notificationPending.exchange(true);
    }
    if (needsTrigger) triggerAsyncUpdate();
}

void ChannelRouteViewSingleton::addListener(juce::MessageListener* listener)
{
    juce::ScopedLock lock(activeListenersLock);
    activeListeners.insert(listener);
    listeners.add(listener);
}

void ChannelRouteViewSingleton::removeListener(juce::MessageListener* listener)
{
    juce::ScopedLock lock(activeListenersLock);
    activeListeners.erase(listener);
    listeners.remove(listener);
}

juce::String ChannelRouteViewSingleton::getChannelUuid() const
{
    juce::ScopedLock lock(stringLock);
    return channelUuid;
}

void ChannelRouteViewSingleton::setChannelUuid(const juce::String& uuid)
{
    juce::ScopedLock lock(stringLock);
    channelUuid = uuid;
}


void ChannelRouteViewSingleton::handleAsyncUpdate()
{
    std::deque<RouteViewState> localQueue;
    {
        juce::ScopedLock lock(activeListenersLock);
        localQueue.swap(stateQueue);
        notificationPending.store(false, std::memory_order_release);
    }

    for (const auto state : localQueue)
    {
        currentState.store(state, std::memory_order_release);

        juce::MessageManager::callAsync([this, state]() {
            ChannelRouteViewMessage msg(static_cast<int>(state));

            listeners.call([this, &msg](juce::MessageListener& l) {
                juce::ScopedLock lock(activeListenersLock);
                if (activeListeners.find(&l) != activeListeners.end()) {
                    l.handleMessage(msg);
                }
                });
            });
    }
}
