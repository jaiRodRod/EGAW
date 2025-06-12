/*
  ==============================================================================

    MixerInterfaceFocusSingleton.cpp
    Created: 21 May 2025 4:38:00pm
    Author:  jaezro

  ==============================================================================
*/

#include "MixerInterfaceFocusSingleton.h"

MixerInterfaceFocusSingleton& MixerInterfaceFocusSingleton::getInstance()
{
	static MixerInterfaceFocusSingleton instance;
	return instance;
}

MixerInterfaceFocusSingleton::MixerInterfaceFocusSingleton() = default;
MixerInterfaceFocusSingleton::~MixerInterfaceFocusSingleton() { cancelPendingUpdate(); }

MixerInterfaceFocusSingleton::MixerFocusState MixerInterfaceFocusSingleton::getCurrentState() const noexcept
{
    return currentState.load(std::memory_order_acquire);
}

void MixerInterfaceFocusSingleton::setState(MixerFocusState newState)
{
    bool needsTrigger = false;
    {
        juce::ScopedLock lock(activeListenersLock);
        stateQueue.push_back(newState);
        needsTrigger = !notificationPending.exchange(true);
    }
    if (needsTrigger) triggerAsyncUpdate();
}

void MixerInterfaceFocusSingleton::addListener(juce::MessageListener* listener)
{
    juce::ScopedLock lock(activeListenersLock);
    activeListeners.insert(listener);
    listeners.add(listener);
}

void MixerInterfaceFocusSingleton::removeListener(juce::MessageListener* listener)
{
    juce::ScopedLock lock(activeListenersLock);
    activeListeners.erase(listener);
    listeners.remove(listener);
}

juce::String MixerInterfaceFocusSingleton::getChannelUuid() const
{
    juce::ScopedLock lock(stringLock);
    return channelUuid;
}

void MixerInterfaceFocusSingleton::setChannelUuid(const juce::String& uuid)
{
    juce::ScopedLock lock(stringLock);
    channelUuid = uuid;
}

void MixerInterfaceFocusSingleton::handleAsyncUpdate()
{
    std::deque<MixerFocusState> localQueue;
    {
        juce::ScopedLock lock(activeListenersLock);
        localQueue.swap(stateQueue);
        notificationPending.store(false, std::memory_order_release);
    }

    for (const auto state : localQueue)
    {
        currentState.store(state, std::memory_order_release);

        juce::MessageManager::callAsync([this, state]() {
            MixerFocusMessage msg(static_cast<int>(state));

            listeners.call([this, &msg](juce::MessageListener& l) {
                juce::ScopedLock lock(activeListenersLock);
                if (activeListeners.find(&l) != activeListeners.end()) {
                    l.handleMessage(msg);
                }
                });
            });
    }
}

