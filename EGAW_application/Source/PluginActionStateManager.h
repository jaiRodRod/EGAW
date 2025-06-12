/*
  ==============================================================================

    PluginActionStateManager.h
    Created: 24 Apr 2025 5:17:27pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <unordered_set>

class PluginMessage : public juce::Message
{
public:
    explicit PluginMessage(int state) : pluginSignal(state) {}
    const int pluginSignal;
};

class PluginActionStateManager : public juce::AsyncUpdater
{
public:
    enum class PluginSignal {
		NULL_SIGNAL,
        ADD_PLUGIN,
		DO_ADD_PLUGIN,
		REMOVE_PLUGIN,
		OPEN_PLUGIN,
		CLOSE_PLUGIN,
    };

    static PluginActionStateManager& getInstance();

    // Thread-safe state access
    PluginSignal getCurrentSignal() const noexcept;
    void setSignal(PluginSignal newState);

    // Thread-safe listener management
    void addListener(juce::MessageListener* listener);
    void removeListener(juce::MessageListener* listener);

    // Thread-safe string access
    juce::String getChannelUuid() const;
    void setChannelUuid(const juce::String& uuid);
    juce::String getPluginUuid() const;
    void setPluginUuid(const juce::String& uuid);
	void setPluginDescription(const juce::PluginDescription& description);
    juce::PluginDescription getPluginDescription() const;
	void setPluginList(juce::KnownPluginList* list);
    juce::KnownPluginList* getPluginList() const;
	void setPluginFormatManager(juce::AudioPluginFormatManager* formatManager);
	juce::AudioPluginFormatManager* getPluginFormatManager() const;

    void clear();

private:
    PluginActionStateManager();
    ~PluginActionStateManager() override;

    void handleAsyncUpdate() override;

    // State management
    std::atomic<PluginSignal> currentSignal{ PluginSignal::NULL_SIGNAL };
    std::deque<PluginSignal> stateQueue;
    std::atomic<bool> notificationPending{ false };

    // String storage
    juce::CriticalSection stringLock;
    juce::String channelUuid;
    juce::String pluginUuid;

	juce::PluginDescription pluginDescription;

    juce::KnownPluginList* pluginList {nullptr};
	juce::AudioPluginFormatManager* pluginFormatManager;

    // Listener management
    juce::ThreadSafeListenerList<juce::MessageListener> listeners;
    std::unordered_set<juce::MessageListener*> activeListeners;
    juce::CriticalSection activeListenersLock;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginActionStateManager)
};