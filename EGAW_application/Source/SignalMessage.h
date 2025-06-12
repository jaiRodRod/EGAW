/*
  ==============================================================================

    SignalMessage.h
    Created: 1 Apr 2025 7:07:04pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SignalMessage : public juce::Message
{
public:
    explicit SignalMessage(int signalType)
        : Message(), signalType(signalType) {
    }

    int getSignalType() const noexcept { return signalType; }

private:
    int signalType;
};