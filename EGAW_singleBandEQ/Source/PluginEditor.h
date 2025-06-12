/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "ColourConstants.h"

#include "VolumeSlider.h"
#include "FrequencySlider.h"

//==============================================================================
/**
*/
class SingleBandEQ_Editor  : public juce::AudioProcessorEditor
{
public:
    SingleBandEQ_Editor (SingleBandEQ&);
    ~SingleBandEQ_Editor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SingleBandEQ& audioProcessor;

	juce::FlexBox gainBox;
	juce::FlexBox frequencyBox;

	juce::FlexBox flexBox;

    // UI components
	juce::Label gainLabel;
    VolumeSlider gainSlider;

	juce::Label frequencyLabel;
    FrequencySlider frequencySlider;

    juce::ComboBox filterTypeBox;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> frequencyAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SingleBandEQ_Editor)
};