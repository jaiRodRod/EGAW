/*
  ==============================================================================

    SignalManagerUI.h
    Created: 19 Feb 2025 1:11:58pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SignalMessage.h"

class SignalManagerUI : public juce::AsyncUpdater
{
public:

    enum class Signal
    {
        NULL_SIGNAL,
        LOAD_FILE,
        SAVE_FILE,
        SAVE_AS_FILE,
        RESTORE_PROJECT_DATA,
        RUN_EXPORT_PROCESS,
        REBUILD_UI,
        RESTORE_UI_PARAMETERS,
        RESIZED_TRIGGER,
        PLAY_AUDIO,
        PAUSE_AUDIO,
        STOP_AUDIO,
        ROUTING_TO_ON,
        ROUTING_TO_OFF,
        ADD_AUDIO_CHANNEL,
        DO_ADD_AUDIO_CHANNEL,
        ADD_MIX_BUS_CHANNEL,
    };

    static SignalManagerUI& getInstance();

    void setSignal(Signal newSignal) {
        setSignalInternal(static_cast<int>(newSignal));
    };

    void addListener(juce::MessageListener* listener);
    void removeListener(juce::MessageListener* listener);

private:
    SignalManagerUI();
    ~SignalManagerUI() override;

    void setSignalInternal(int signalId);

    void handleAsyncUpdate() override;

    juce::ThreadSafeListenerList<juce::MessageListener> listeners;
    juce::CriticalSection queueLock;
    std::deque<int> signalQueue; // Stores raw signal IDs
    std::atomic<bool> notificationPending{ false };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SignalManagerUI)
};