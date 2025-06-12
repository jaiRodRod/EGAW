/*
  ==============================================================================

    ChannelRouteViewSingleton.h
    Created: 23 May 2025 5:07:36pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <unordered_set>

class ChannelRouteViewMessage : public juce::Message
{
public:
    explicit ChannelRouteViewMessage(int state) : viewState(state) {}
    const int viewState;
};

class ChannelRouteViewSingleton : public juce::AsyncUpdater
{
public:
    enum class RouteViewState {
        VIEWING_OFF,
        VIEWING_ON,
    };

    static ChannelRouteViewSingleton& getInstance();

    // Thread-safe state access
    RouteViewState getCurrentState() const noexcept;
    void setState(RouteViewState newState);

    // Thread-safe listener management
    void addListener(juce::MessageListener* listener);
    void removeListener(juce::MessageListener* listener);

    // Thread-safe string access
    juce::String getChannelUuid() const;
    void setChannelUuid(const juce::String& uuid);

private:
    ChannelRouteViewSingleton();
    ~ChannelRouteViewSingleton() override;

    void handleAsyncUpdate() override;

    // State management
    std::atomic<RouteViewState> currentState{ RouteViewState::VIEWING_OFF };
    std::deque<RouteViewState> stateQueue;
    std::atomic<bool> notificationPending{ false };

    // String storage
    juce::CriticalSection stringLock;
    juce::String channelUuid;
    //juce::String destinyChannelUuid;

    // Listener management
    juce::ThreadSafeListenerList<juce::MessageListener> listeners;
    std::unordered_set<juce::MessageListener*> activeListeners;
    juce::CriticalSection activeListenersLock;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChannelRouteViewSingleton)
};