/*
  ==============================================================================

    AudioChannel.h
    Created: 9 Feb 2025 12:57:09pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Channel.h"

#include "SignalManagerUI.h"
#include "SoloControlSingleton.h"
#include "GlobalPlayhead.h"

class AudioChannel : public Channel, public juce::ValueTree::Listener, public juce::Value::Listener
{
public:

    AudioChannel();
    AudioChannel(juce::String&, juce::ValueTree&); //Constructor para reconstruir con los parametros cuando se reconstruya la serializacion
    ~AudioChannel();

    void loadFile();
    bool isFileLoaded() const;
    juce::File getLoadedFile() const;

    //Funciones para que herede correctamente de AudioSource
    void prepareToPlay(int, double) override;
    void releaseResources() override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo&);

    //Funciones para que herede correctamente de PositionableAudioSource
    void setNextReadPosition(juce::int64) override;
    juce::int64 getNextReadPosition() const override;
    juce::int64 getTotalLength() const override;
    bool isLooping() const override;
    void setLooping(bool) override;

    void setStartTime(double seconds); 

    /*
    float getGain() const;          
    void setGain(float gainValue);  //Encapsular para hacerlo undoable
    float getPan() const;
    void setPan(float panValue);    //Encapsular para hacerlo undoable
    bool isSolo() const;
    void setSolo(bool soloValue);   //Encapsular para hacerlo undoable
    */

    juce::ValueTree& getValueTree();
    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override;
    void valueChanged(juce::Value& value) override;

    bool isSoloOn() { return channelSolo; };

private:
    
    bool loadFileInternal(const juce::File&);
    void setMute(bool muteValue); //Hacerlo undoable
    void setSolo(bool soloValue); //Hacerlo undoable
    void setSoloMute();

    std::atomic<juce::int64> startSample{ 0 };
    std::atomic<bool> isActive{ false };
    double sampleRate = 48000.0;
    double fileSampleRate = 48000.0;

    std::unique_ptr<juce::FileChooser> fileChooser; //Not needed to re-init

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    std::unique_ptr<juce::ResamplingAudioSource> resampler;
    juce::File currentFile;
    bool isPrepared = false;

    //Propiedades del AudioChannel:
    // Type -> AudioChannel
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
};