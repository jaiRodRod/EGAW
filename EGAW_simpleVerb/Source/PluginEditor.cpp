/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"

ReverbPluginAudioProcessorEditor::ReverbPluginAudioProcessorEditor(ReverbPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
	, mainLayout(juce::FlexBox::Direction::row, juce::FlexBox::Wrap::noWrap, juce::FlexBox::AlignContent::center,
        juce::FlexBox::AlignItems::center, juce::FlexBox::JustifyContent::center),
	verticalNotches(juce::FlexBox::Direction::column, juce::FlexBox::Wrap::noWrap, juce::FlexBox::AlignContent::center,
        juce::FlexBox::AlignItems::center, juce::FlexBox::JustifyContent::center)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(450, 600);
	setResizable(true, true);

    addAndMakeVisible(dryLevelSlider);
    dryLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "dryLevel", dryLevelSlider);

    addAndMakeVisible(wetLevelSlider);
    wetLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "wetLevel", wetLevelSlider);

    addAndMakeVisible(dampingComponent);
    dampingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "damping", dampingComponent.getDampingSlider());

    addAndMakeVisible(roomSizeComponent);
    roomSizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "roomSize", roomSizeComponent.getRoomSizeSlider());

    addAndMakeVisible(widthComponent);
    widthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "width", widthComponent.getWidthSlider());
}

ReverbPluginAudioProcessorEditor::~ReverbPluginAudioProcessorEditor()
{
}

void ReverbPluginAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(colourConstants::dark_slate_blue);
}

void ReverbPluginAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(10);
    mainLayout.items.clear();
	verticalNotches.items.clear();

	mainLayout.items.add(juce::FlexItem(dryLevelSlider).withMinHeight(area.getHeight() * 0.9).withMinWidth(area.getWidth() / 3));
	mainLayout.items.add(juce::FlexItem(wetLevelSlider).withMinHeight(area.getHeight() * 0.9).withMinWidth(area.getWidth() / 3));

    verticalNotches.items.add(juce::FlexItem(dampingComponent).withMinHeight(area.getHeight() / 3).withMinWidth(area.getWidth() / 3));
	verticalNotches.items.add(juce::FlexItem(roomSizeComponent).withMinHeight(area.getHeight() / 3).withMinWidth(area.getWidth() / 3));
	verticalNotches.items.add(juce::FlexItem(widthComponent).withMinHeight(area.getHeight() / 3).withMinWidth(area.getWidth() / 3));

	mainLayout.items.add(juce::FlexItem(verticalNotches).withMinHeight(area.getHeight()).withMinWidth(area.getWidth() / 3));

	mainLayout.performLayout(area);
}