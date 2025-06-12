/*
  ==============================================================================

    PluginActionStateManager.cpp
    Created: 24 Apr 2025 5:17:27pm
    Author:  jaezro

  ==============================================================================
*/

#include "PluginActionStateManager.h"

PluginActionStateManager& PluginActionStateManager::getInstance()
{
    static PluginActionStateManager instance;
    return instance;
}

PluginActionStateManager::PluginActionStateManager() = default;
PluginActionStateManager::~PluginActionStateManager() { cancelPendingUpdate(); }

// Thread-safe state management
PluginActionStateManager::PluginSignal PluginActionStateManager::getCurrentSignal() const noexcept
{
    return currentSignal.load(std::memory_order_acquire);
}

void PluginActionStateManager::setSignal(PluginSignal newState)
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
void PluginActionStateManager::addListener(juce::MessageListener* listener)
{
    juce::ScopedLock lock(activeListenersLock);
    activeListeners.insert(listener);
    listeners.add(listener);
}

void PluginActionStateManager::removeListener(juce::MessageListener* listener)
{
    juce::ScopedLock lock(activeListenersLock);
    activeListeners.erase(listener);
    listeners.remove(listener);
}

// Thread-safe string access
juce::String PluginActionStateManager::getChannelUuid() const
{
    juce::ScopedLock lock(stringLock);
    return channelUuid;
}

void PluginActionStateManager::setChannelUuid(const juce::String& uuid)
{
    juce::ScopedLock lock(stringLock);
    channelUuid = uuid;
}

juce::String PluginActionStateManager::getPluginUuid() const
{
    juce::ScopedLock lock(stringLock);
    return pluginUuid;
}

void PluginActionStateManager::setPluginUuid(const juce::String& uuid)
{
    juce::ScopedLock lock(stringLock);
    pluginUuid = uuid;
}

void PluginActionStateManager::setPluginDescription(const juce::PluginDescription& description)
{
	juce::ScopedLock lock(stringLock);
	pluginDescription = description;
}

juce::PluginDescription PluginActionStateManager::getPluginDescription() const
{
    juce::ScopedLock lock(stringLock);
    return pluginDescription;
}

void PluginActionStateManager::setPluginList(juce::KnownPluginList* list)
{
	juce::ScopedLock lock(stringLock);
	pluginList = list;
}

juce::KnownPluginList* PluginActionStateManager::getPluginList() const
{
	juce::ScopedLock lock(stringLock);
	return pluginList;
}

void PluginActionStateManager::setPluginFormatManager(juce::AudioPluginFormatManager* formatManager)
{
	juce::ScopedLock lock(stringLock);
	pluginFormatManager = formatManager;
}

juce::AudioPluginFormatManager* PluginActionStateManager::getPluginFormatManager() const
{
	juce::ScopedLock lock(stringLock);
	return pluginFormatManager;
}

void PluginActionStateManager::clear()
{
    setSignal(PluginSignal::NULL_SIGNAL);
    setChannelUuid({});
    setPluginUuid({});
	setPluginDescription({});
}

// Core message delivery
void PluginActionStateManager::handleAsyncUpdate()
{
    std::deque<PluginSignal> localQueue;
    {
        juce::ScopedLock lock(activeListenersLock);
        localQueue.swap(stateQueue);
        notificationPending.store(false, std::memory_order_release);
    }

    for (const auto state : localQueue)
    {
        currentSignal.store(state, std::memory_order_release);

        juce::MessageManager::callAsync([this, state]() {
            PluginMessage msg(static_cast<int>(state));

            listeners.call([this, &msg](juce::MessageListener& l) {
                juce::ScopedLock lock(activeListenersLock);
                if (activeListeners.find(&l) != activeListeners.end()) {
                    l.handleMessage(msg);
                }
                });
            });
    }
}