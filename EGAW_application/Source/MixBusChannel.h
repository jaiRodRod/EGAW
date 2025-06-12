/*
  ==============================================================================

    MixBusChannel.h
    Created: 10 Feb 2025 11:28:15am
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RoutableChannel.h"
#include "SoloControlSingleton.h"

#include "ColourConstants.h"

class MixBusChannel : public RoutableChannel, public juce::ValueTree::Listener, public juce::Value::Listener
{
public:
    MixBusChannel();
    MixBusChannel(juce::String, juce::ValueTree&);
    ~MixBusChannel();

    // Implementacie los m	todos de juce::AudioSource
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    //Implementacion de los metodos de juce::AudioPositionableSource
    ///Todo: Revisar estos metodos y comprobar que vaya todo bien
    void setNextReadPosition(juce::int64 newPosition) override;
    juce::int64 getNextReadPosition() const override;
    juce::int64 getTotalLength() const override;
    bool isLooping() const override;

    // M	todos adicionales para manejar el enrutamiento
    void routeIn(Channel*) override;
    void removeRouteIn(Channel*) override;
    void clearMixBusChannel();

    juce::ValueTree& getValueTree();
    void setSettings(juce::ValueTree);

    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override;
    void valueChanged(juce::Value& value) override;

    bool isSoloOn() { return channelSolo; };
    ///Todo: 
    /// - Hacer un correcto manejo de los metodos de AudioPositionableSource

private:

    juce::int64 getMaxLengthFromChannels() const;
    void setMute(bool muteValue);
    void setSolo(bool soloValue); //Hacerlo undoable
    void setSoloMute();

    juce::MixerAudioSource mixerSource;
    juce::int64 nextReadPosition = 0;

    //Propiedades del MixBusChannel:
    // Type -> "MixBusChannel"
    // Volumen -> "Gain" (init = 0.0)
    // Panning -> "Pan" (init = 0.0)
    // File location -> "FilePath"
    // Mute -> "Mute" (init = false)
    // Solo -> "Solo" (init = false)
    juce::ValueTree settings; //Las setings del canal tendran como Identifier cuando se construya el UUID generado en la clase Channel
    juce::dsp::Gain<float> gain;
    float panValue;
    juce::dsp::Panner<float> pan;
    bool channelMute;
    bool channelSolo;
    bool isGlobalSoloActive;

    bool hasBeenReleased = false;
};