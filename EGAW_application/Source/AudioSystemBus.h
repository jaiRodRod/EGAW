/*
  ==============================================================================

    AudioSystemBus.h
    Created: 10 Feb 2025 4:45:15pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "AudioChannel.h"
#include "MixBusChannel.h"
#include "PluginRackChannel.h"
#include "SignalManagerUI.h"
#include "SoloControlSingleton.h"
#include "RoutingActionStateManager.h"
#include "GlobalPlayhead.h"

class AudioSystemBus : public juce::PositionableAudioSource, public juce::ValueTree::Listener, public juce::MessageListener
{

public:

    AudioSystemBus(juce::AudioDeviceManager::AudioDeviceSetup&, juce::ValueTree&, juce::ValueTree&, GlobalPlayhead&);
    ~AudioSystemBus();

    void valueTreeChildRemoved(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override;
	void handleMessage(const juce::Message& message) override;

    // Implementacion de los metodos de juce::AudioSource
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    //Implementacion de los metodos de juce::AudioPositionableSource
    void setNextReadPosition(juce::int64 newPosition) override;
    juce::int64 getNextReadPosition() const override;
    juce::int64 getTotalLength() const override;
    bool isLooping() const override;

    void addMixBusChannel();
    void removeMixBusChannel(MixBusChannel*, juce::ValueTree);
    juce::Array<MixBusChannel*> getMixBusChannels();

    void addPluginRackChannel();
    void removePluginRackChannel(PluginRackChannel*, juce::ValueTree);
    juce::Array<PluginRackChannel*> getPluginRackChannels();

    /// <summary>
    /// Metodo para crear un canal en el proyecto actual. 
    /// Este metodo no se usa en la reconstruccion de un proyecto
    /// </summary>
    void addAudioChannel();
    void removeAudioChannel(AudioChannel*, juce::ValueTree);
    juce::Array<AudioChannel*> getAudioChannels();

    void routeInMasterBusChannel(Channel*);         //Encapsular para hacerlo undoable
    void removeRouteInMasterBusChannel(Channel*);   //Encapsular para hacerlo undoable

    juce::ValueTree& getProjectData();

    juce::int64 getTotalSamples();
    void setAudioDeviceSetup(juce::AudioDeviceManager::AudioDeviceSetup newAudioDeviceSetup);
    void processNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);

    void start();
    void setTransportToBegin();
    juce::AudioTransportSource& getTransportSource() { return transportSource; };

private:


    juce::ValueTree projectData;
    juce::ValueTree fileRestoreProjectData;

    void restoreProjectData();
    Channel* getChannelByUUID(juce::String);
    RoutableChannel* getRoutableChannelByUUID(juce::String);
    void clearAudioSystemBusState();

    /// <summary>
    /// Usado de forma interna por addAudioChannel, cuando se cree el canal,
    /// se lanzar desde el nuevo AudioChannel una seal que activa este metodo.
    /// </summary>
    void doAddAudioChannel();

    void doRouting();
    void doRemoveRoute();

    //double bpm;
    //int beatNumerator;
    //int beatDivider;

    bool playing;
    juce::AudioTransportSource transportSource;

    MixBusChannel masterBusChannel;
    juce::OwnedArray<MixBusChannel> mixBusChannels;
    juce::OwnedArray<PluginRackChannel> pluginRackChannels;
    juce::OwnedArray<AudioChannel> audioChannels;
    juce::ValueTree channelOrder;

    AudioChannel* tempAudioChannel = nullptr;

    juce::AudioDeviceManager::AudioDeviceSetup audioDeviceSetup;
    GlobalPlayhead& globalPlayhead;
};