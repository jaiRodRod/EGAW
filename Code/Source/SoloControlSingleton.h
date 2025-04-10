/*
  ==============================================================================

    SoloControlSingleton.h
    Created: 4 Mar 2025 9:37:23am
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SoloControlSingleton : public juce::Value::Listener
{
public:

    enum class SoloSignal
    {
        NULL_SIGNAL,
        GLOBAL_SOLO_ON,
        GLOBAL_SOLO_OFF
    };

    SoloSignal getCurrentSignal() const;
    void setSignal(SoloSignal newSignal);
    void addListener(juce::Value::Listener* listener);
    void removeListener(juce::Value::Listener* listener);

    bool checkIsGlobalSoloActive() { return isGlobalSoloActive; };
    void resetSingleton();

    JUCE_DECLARE_SINGLETON(SoloControlSingleton, false);


private:

    SoloControlSingleton();
    ~SoloControlSingleton() { clearSingletonInstance(); };

    void valueChanged(juce::Value& value) override
    {
        //Se declara para deshacer la abstraccion pero no se utilizará nada
    }

    juce::Value soloSignal;
    bool isGlobalSoloActive;
    int numSoloChannels;
};