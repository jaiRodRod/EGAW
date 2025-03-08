/*
  ==============================================================================

    SignalManagerUI.h
    Created: 19 Feb 2025 1:11:58pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SignalManagerUI : public juce::Value::Listener
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

    Signal getCurrentSignal() const;
    void setSignal(Signal newSignal);
    void addListener(juce::Value::Listener* listener);
    void removeListener(juce::Value::Listener* listener);

    juce::Value getValue() { return signalValue; };

    JUCE_DECLARE_SINGLETON(SignalManagerUI, true);

private:

    SignalManagerUI();

    void valueChanged(juce::Value& value) override
    {
        //Se declara para deshacer la abstraccion pero no se utilizará nada
    }


    juce::Value signalValue;
};