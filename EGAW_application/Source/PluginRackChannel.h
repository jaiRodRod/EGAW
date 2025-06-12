/*
  ==============================================================================

    PluginRackChannel.h
    Created: 14 Apr 2025 4:38:16pm
    Author:  jaezro

  ==============================================================================
*/


#include <JuceHeader.h>
#include "RoutableChannel.h"

#include "PluginList.h"

#include "SoloControlSingleton.h"

#include "ColourConstants.h"


class PluginRackChannel : public RoutableChannel
                        // , public juce::AudioProcessorListener
                        , public juce::ValueTree::Listener
                        , public juce::Value::Listener
{
public:
    PluginRackChannel(GlobalPlayhead&);
    PluginRackChannel(GlobalPlayhead&, juce::String channelId, juce::ValueTree& settings);
    ~PluginRackChannel();

    // AudioSource overrides
    void prepareToPlay(int samplesPerBlock, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    // PositionableAudioSource
    void setNextReadPosition(juce::int64 newPosition) override;
    juce::int64 getNextReadPosition() const override;
    juce::int64 getTotalLength() const override;
    bool isLooping() const override;

    void routeIn(Channel*) override;
    void removeRouteIn(Channel*) override;
    void clearPluginRackChannel();

    juce::ValueTree& getSettings();
    void setSettings(juce::ValueTree);

    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override;
    void valueChanged(juce::Value& value) override;

    bool isSoloOn() { return channelSolo; };

private:

    juce::int64 getMaxLengthFromChannels() const;
    void setMute(bool muteValue);
    void setSolo(bool soloValue); //Hacerlo undoable
    void setSoloMute();

    juce::MixerAudioSource mixerSource;
    juce::int64 nextReadPosition = 0;

    juce::ValueTree settings; //Las setings del canal tendran como Identifier cuando se construya el UUID generado en la clase Channel
    bool channelMute;
    bool channelSolo;
    bool isGlobalSoloActive;

    juce::ValueTree pluginState;
	std::unique_ptr<PluginList> pluginList;

    bool hasBeenReleased;
};