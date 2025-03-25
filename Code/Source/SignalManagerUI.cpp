/*
  ==============================================================================

    SignalManagerUI.cpp
    Created: 19 Feb 2025 1:11:58pm
    Author:  jaezro

  ==============================================================================
*/

#include "SignalManagerUI.h"

JUCE_IMPLEMENT_SINGLETON(SignalManagerUI);

SignalManagerUI::SignalManagerUI()
{
    signalValue = static_cast<int>(Signal::NULL_SIGNAL);
}

SignalManagerUI::Signal SignalManagerUI::getCurrentSignal() const
{
    return static_cast<Signal>(static_cast<int>(signalValue.getValue()));
}

void SignalManagerUI::setSignal(Signal newSignal)
{
    juce::MessageManager::callAsync([this, newSignal]()
    {
        signalValue = static_cast<int>(newSignal); // Ensure it's set on UI thread
    });
}

void SignalManagerUI::addListener(juce::Value::Listener* listener)
{
    const juce::ScopedLock lock(listenerLock);
    signalValue.addListener(listener);
}

void SignalManagerUI::removeListener(juce::Value::Listener* listener)
{
    const juce::ScopedLock lock(listenerLock);
    signalValue.removeListener(listener);
}
