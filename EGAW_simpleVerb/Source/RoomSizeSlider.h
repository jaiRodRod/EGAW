/*
  ==============================================================================

    RoomSizeSlider.h
    Created: 27 May 2025 6:28:37pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class RoomSizeSlider : public juce::Slider
{
public:
    RoomSizeSlider();
    ~RoomSizeSlider() override;

    void paint (juce::Graphics&) override;
    //void resized() override;
    
    //Metodo para mostrar datos en las parametrizaciones
    void setDataMode(bool shouldBeOn);


private:

    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseDoubleClick(const juce::MouseEvent& event) override;
    void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel) override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;

    juce::Label roomSizeLevelLabel;
    bool isTextActive;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RoomSizeSlider);
};