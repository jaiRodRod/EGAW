/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "ColourConstants.h"

#include "DrySlider.h"
#include "WetSlider.h"

#include "DampingComponent.h"
#include "RoomSizeComponent.h"
#include "WidthComponent.h"

class ReverbPluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    ReverbPluginAudioProcessorEditor(ReverbPluginAudioProcessor&);
    ~ReverbPluginAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    ReverbPluginAudioProcessor& audioProcessor;

	juce::FlexBox mainLayout;
	juce::FlexBox verticalNotches;

    // Sliders and attachments
    WetSlider wetLevelSlider;
    DrySlider dryLevelSlider;

    DampingComponent dampingComponent;
    RoomSizeComponent roomSizeComponent;
    WidthComponent widthComponent;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> roomSizeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dampingAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> wetLevelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dryLevelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> widthAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbPluginAudioProcessorEditor)
};