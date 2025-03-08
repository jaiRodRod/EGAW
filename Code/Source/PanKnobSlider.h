/*
  ==============================================================================

    PanKnobSlider.h
    Created: 5 Mar 2025 9:31:31am
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "SignalManagerUI.h"

//==============================================================================
/*
*/
class PanKnobSlider  : public juce::Slider, public juce::Value::Listener
{
public:
    PanKnobSlider(juce::ValueTree&);
    ~PanKnobSlider() override;

    //void paint (juce::Graphics&) override;
    //void resized() override;

    void valueChanged(juce::Value& value) override;

    //Metodo para mostrar datos en las parametrizaciones
    void setDataMode(bool shouldBeOn);


private:

    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseDoubleClick(const juce::MouseEvent& event) override;
    void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel) override;
    void mouseDown(const juce::MouseEvent& event) override;

    juce::ValueTree channelSettings;
    bool isTextActive;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PanKnobSlider)
};
