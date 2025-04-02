/*
  ==============================================================================

    SignalManagerUI.cpp
    Created: 19 Feb 2025 1:11:58pm
    Author:  jaezro

  ==============================================================================
*/

#include "SignalManagerUI.h"

SignalManagerUI& SignalManagerUI::getInstance()
{
    static SignalManagerUI instance;
    return instance;
}

SignalManagerUI::SignalManagerUI() = default;
SignalManagerUI::~SignalManagerUI() { cancelPendingUpdate(); }

void SignalManagerUI::setSignalInternal(int newSignal)
{
    bool needsTrigger = false;
    {
        const juce::ScopedLock lock(queueLock);
        if (signalQueue.empty() || signalQueue.back() != newSignal)
        {
            signalQueue.push_back(newSignal);
            needsTrigger = !notificationPending.exchange(true);
        }
    }

    if (needsTrigger)
        triggerAsyncUpdate();
}

void SignalManagerUI::handleAsyncUpdate()
{
    std::deque<int> localQueue;
    {
        const juce::ScopedLock lock(queueLock);
        localQueue.swap(signalQueue);
        notificationPending.store(false);
    }

    // Process on message thread
    juce::MessageManager::callAsync([this, localQueue = std::move(localQueue)]()
        {
            for (const auto signal : localQueue)
            {
                SignalMessage msg(signal); // Properly constructed
                listeners.call([&msg](juce::MessageListener& l) {
                    l.handleMessage(msg); // Correct message passing
                    });
            }
        });
}

void SignalManagerUI::addListener(juce::MessageListener* listener)
{
    listeners.add(listener);
}

void SignalManagerUI::removeListener(juce::MessageListener* listener)
{
    listeners.remove(listener);
}