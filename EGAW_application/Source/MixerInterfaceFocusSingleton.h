/*
  ==============================================================================

    MixerInterfaceFocusSingleton.h
    Created: 21 May 2025 4:38:00pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <unordered_set>

class MixerFocusMessage : public juce::Message
{
public:
    explicit MixerFocusMessage(int state) : focusState(state) {}
    const int focusState;
};

class MixerInterfaceFocusSingleton : public juce::AsyncUpdater
{
public:
    enum class MixerFocusState {
        FOCUS_OFF,
        FOCUS_ON,
    };

    static MixerInterfaceFocusSingleton& getInstance();

    // Thread-safe state access
    MixerFocusState getCurrentState() const noexcept;
    void setState(MixerFocusState newState);

    // Thread-safe listener management
    void addListener(juce::MessageListener* listener);
    void removeListener(juce::MessageListener* listener);

    // Thread-safe string access
    juce::String getChannelUuid() const;
    void setChannelUuid(const juce::String& uuid);

private:
    MixerInterfaceFocusSingleton();
    ~MixerInterfaceFocusSingleton() override;

    void handleAsyncUpdate() override;

    // State management
    std::atomic<MixerFocusState> currentState{ MixerFocusState::FOCUS_OFF };
    std::deque<MixerFocusState> stateQueue;
    std::atomic<bool> notificationPending{ false };

    // String storage
    juce::CriticalSection stringLock;
    juce::String channelUuid;
    //juce::String destinyChannelUuid;

    // Listener management
    juce::ThreadSafeListenerList<juce::MessageListener> listeners;
    std::unordered_set<juce::MessageListener*> activeListeners;
    juce::CriticalSection activeListenersLock;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MixerInterfaceFocusSingleton)
};