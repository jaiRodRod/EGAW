/*
  ==============================================================================

    SoloControlSingleton.cpp
    Created: 4 Mar 2025 9:37:23am
    Author:  jaezro

  ==============================================================================
*/

#include "SoloControlSingleton.h"

JUCE_IMPLEMENT_SINGLETON(SoloControlSingleton);

SoloControlSingleton::SoloControlSingleton()
{
    soloSignal = static_cast<int>(SoloSignal::NULL_SIGNAL);
    isGlobalSoloActive = false;
    numSoloChannels = 0;
}

SoloControlSingleton::SoloSignal SoloControlSingleton::getCurrentSignal() const
{
    return static_cast<SoloSignal>(static_cast<int>(soloSignal.getValue()));
}

void SoloControlSingleton::setSignal(SoloSignal newSignal)
{
    soloSignal = static_cast<int>(newSignal);

    switch (newSignal)
    {
    case SoloControlSingleton::SoloSignal::NULL_SIGNAL:
        break;
    case SoloControlSingleton::SoloSignal::GLOBAL_SOLO_ON:
        isGlobalSoloActive = true;
        ++numSoloChannels;
        SoloControlSingleton::getInstance()->setSignal(SoloControlSingleton::SoloSignal::NULL_SIGNAL);
        break;
    case SoloControlSingleton::SoloSignal::GLOBAL_SOLO_OFF:
        --numSoloChannels;
        if (numSoloChannels <= 0)
        {
            if (numSoloChannels < 0)
            {
                numSoloChannels = 0;
            }
            isGlobalSoloActive = false;
        }
        SoloControlSingleton::getInstance()->setSignal(SoloControlSingleton::SoloSignal::NULL_SIGNAL);
        break;
    default:
        break;
    }

    DBG("NUMERO DE SOLO CHANNELS: " << numSoloChannels);
}

void SoloControlSingleton::addListener(juce::Value::Listener* listener)
{
    soloSignal.addListener(listener);
}

void SoloControlSingleton::removeListener(juce::Value::Listener* listener)
{
    soloSignal.removeListener(listener);
}

void SoloControlSingleton::resetSingleton()
{
    isGlobalSoloActive = false;
    numSoloChannels = 0;
    setSignal(SoloSignal::NULL_SIGNAL);
}
