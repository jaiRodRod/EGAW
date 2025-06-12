/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

ReverbPluginAudioProcessor::ReverbPluginAudioProcessor()
    : parameters(*this, nullptr, "Parameters", {
        std::make_unique<juce::AudioParameterFloat>(
            "roomSize", "Room Size",
            juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f),
        std::make_unique<juce::AudioParameterFloat>(
            "damping", "Damping",
            juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f),
        std::make_unique<juce::AudioParameterFloat>(
            "wetLevel", "Wet Level",
            juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 1.0f),
        std::make_unique<juce::AudioParameterFloat>(
            "dryLevel", "Dry Level",
            juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 1.0f),
        std::make_unique<juce::AudioParameterFloat>(
            "width", "Width",
            juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f)
        })
{
}

ReverbPluginAudioProcessor::~ReverbPluginAudioProcessor()
{
}

void ReverbPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    reverb.setSampleRate(sampleRate);
}

void ReverbPluginAudioProcessor::releaseResources()
{
}

void ReverbPluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    // Update reverb parameters
    auto& roomSize = *parameters.getRawParameterValue("roomSize");
    auto& damping = *parameters.getRawParameterValue("damping");
    auto& wetLevel = *parameters.getRawParameterValue("wetLevel");
    auto& dryLevel = *parameters.getRawParameterValue("dryLevel");
    auto& width = *parameters.getRawParameterValue("width");

    reverbParams.roomSize = roomSize;
    reverbParams.damping = damping;
    reverbParams.wetLevel = wetLevel;
    reverbParams.dryLevel = dryLevel;
    reverbParams.width = width;
    reverb.setParameters(reverbParams);

    reverb.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples());
}

juce::AudioProcessorEditor* ReverbPluginAudioProcessor::createEditor()
{
    return new ReverbPluginAudioProcessorEditor(*this);
}

void ReverbPluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void ReverbPluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverbPluginAudioProcessor();
}