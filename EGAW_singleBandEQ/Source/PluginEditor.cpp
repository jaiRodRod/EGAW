/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SingleBandEQ_Editor::SingleBandEQ_Editor (SingleBandEQ& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
    , gainBox(juce::FlexBox::Direction::column, juce::FlexBox::Wrap::noWrap, juce::FlexBox::AlignContent::center
    , juce::FlexBox::AlignItems::center, juce::FlexBox::JustifyContent::center)
	, frequencyBox(juce::FlexBox::Direction::row, juce::FlexBox::Wrap::noWrap, juce::FlexBox::AlignContent::center
		, juce::FlexBox::AlignItems::center, juce::FlexBox::JustifyContent::center)
	, flexBox(juce::FlexBox::Direction::column, juce::FlexBox::Wrap::noWrap, juce::FlexBox::AlignContent::center
    , juce::FlexBox::AlignItems::center, juce::FlexBox::JustifyContent::center)
	, gainSlider(), frequencySlider()
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (450, 600);
	setResizable(false, false);

    // Gain slider setup
	addAndMakeVisible(gainLabel);
	gainLabel.setText("Gain", juce::dontSendNotification);
	gainLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(gainSlider);
    gainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        audioProcessor.parameters, "gain", gainSlider));

    // Frequency slider setup
	addAndMakeVisible(frequencyLabel);
	frequencyLabel.setText("Frequency", juce::dontSendNotification);
	frequencyLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(frequencySlider);
    frequencyAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        audioProcessor.parameters, "frequency", frequencySlider));

    // Filter type combo box
    filterTypeBox.addItem("Low Pass", 1);
    filterTypeBox.addItem("High Pass", 2);
    filterTypeBox.addItem("Band Pass", 3);
    filterTypeBox.addItem("Notch", 4);
    filterTypeBox.addItem("Peak", 5);
    filterTypeBox.addItem("Low Shelf", 6);
    filterTypeBox.addItem("High Shelf", 7);
	filterTypeBox.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(filterTypeBox);
    filterTypeAttachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(
        audioProcessor.parameters, "filterType", filterTypeBox));
}

SingleBandEQ_Editor::~SingleBandEQ_Editor()
{
}

//==============================================================================
void SingleBandEQ_Editor::paint (juce::Graphics& g)
{
    g.fillAll(colourConstants::dark_slate_blue);

    //g.setColour(juce::Colours::white);
    //g.setFont(15.0f);
    //g.drawFittedText("Frequency", frequencySlider.getBounds().translated(0, -25), juce::Justification::centred, 1);
    //g.drawFittedText("Gain", gainSlider.getBounds().translated(0, -25), juce::Justification::centred, 1);
    //g.drawFittedText("Filter Type", filterTypeBox.getBounds().translated(0, -25), juce::Justification::centred, 1);
}

void SingleBandEQ_Editor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto area = getLocalBounds().reduced(10);

	gainBox.items.clear();
	gainBox.items.add(juce::FlexItem(gainLabel).withMinHeight(area.getHeight() * 0.05f).withMinWidth(area.getWidth()));
	gainBox.items.add(juce::FlexItem(gainSlider).withMinHeight(area.getHeight() * 0.6f).withMinWidth(area.getWidth()));

	frequencyBox.items.clear();
	frequencyBox.items.add(juce::FlexItem(frequencyLabel).withMinHeight(area.getHeight() * 0.2f).withMinWidth(area.getWidth() * 0.2f));
	frequencyBox.items.add(juce::FlexItem(frequencySlider).withMinHeight(area.getHeight() * 0.2f).withMinWidth(area.getWidth() * 0.6f));

	flexBox.items.clear();

    flexBox.items.add(juce::FlexItem(gainBox).withMinHeight(area.getHeight() * 0.65f).withMinWidth(area.getWidth()));
	flexBox.items.add(juce::FlexItem(frequencyBox).withMinHeight(area.getHeight() * 0.2f).withMinWidth(area.getWidth() * 0.8f));
	flexBox.items.add(juce::FlexItem().withMinHeight(area.getHeight() * 0.05f)); // Spacer
	flexBox.items.add(juce::FlexItem(filterTypeBox).withMinHeight(area.getHeight() * 0.1f).withMinWidth(area.getWidth() * 0.2f));

	flexBox.performLayout(getLocalBounds());
}
