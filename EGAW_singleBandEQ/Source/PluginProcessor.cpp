/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SingleBandEQ::SingleBandEQ()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
    ),
#endif
    parameters(*this, nullptr, "Parameters", {
        std::make_unique<juce::AudioParameterFloat>(
            "frequency", "Frequency",
            juce::NormalisableRange<float>(20.0f, 20000.0f, 0.1f, 0.3f),
            1000.0f),

        std::make_unique<juce::AudioParameterFloat>(
            "gain", "Gain",
            juce::NormalisableRange<float>(-24.0f, 24.0f, 0.1f),
            0.0f),

        std::make_unique<juce::AudioParameterChoice>(
            "filterType", "Filter Type",
            juce::StringArray("Low Pass", "High Pass", "Band Pass", "Notch", "Peak", "Low Shelf", "High Shelf"),
            4) // Default to Peak filter
        })
{
    // Initialize DSP
    filter.prepare({ getSampleRate(), (juce::uint32)getBlockSize(), (juce::uint32)getTotalNumInputChannels() });
}

SingleBandEQ::~SingleBandEQ()
{
}

//==============================================================================
const juce::String SingleBandEQ::getName() const
{
    return JucePlugin_Name;
}

bool SingleBandEQ::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SingleBandEQ::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SingleBandEQ::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SingleBandEQ::getTailLengthSeconds() const
{
    return 0.0;
}

int SingleBandEQ::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SingleBandEQ::getCurrentProgram()
{
    return 0;
}

void SingleBandEQ::setCurrentProgram (int index)
{
}

const juce::String SingleBandEQ::getProgramName (int index)
{
    return {};
}

void SingleBandEQ::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SingleBandEQ::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();

    filter.prepare(spec);
    updateFilter();
}

void SingleBandEQ::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SingleBandEQ::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SingleBandEQ::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    // Update filter if parameters changed
    if (parameters.getParameter("frequency")->getValue() != lastFrequency ||
        parameters.getParameter("gain")->getValue() != lastGain ||
        parameters.getParameter("filterType")->getValue() != lastType)
    {
        updateFilter();
    }

    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    filter.process(context);
}

//==============================================================================
bool SingleBandEQ::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SingleBandEQ::createEditor()
{
    return new SingleBandEQ_Editor (*this);
}

//==============================================================================
void SingleBandEQ::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SingleBandEQ::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void SingleBandEQ::updateFilter()
{
    float frequency = parameters.getRawParameterValue("frequency")->load();
    float gain = parameters.getRawParameterValue("gain")->load();
    int type = parameters.getRawParameterValue("filterType")->load();

    lastFrequency = frequency;
    lastGain = gain;
    lastType = type;

    float Q = 1.0f; // You could make this a parameter too if you want

    switch (static_cast<FilterType>(type))
    {
    case LowPass:
        *filter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), frequency, Q);
        break;

    case HighPass:
        *filter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), frequency, Q);
        break;

    case BandPass:
        *filter.state = *juce::dsp::IIR::Coefficients<float>::makeBandPass(getSampleRate(), frequency, Q);
        break;

    case Notch:
        *filter.state = *juce::dsp::IIR::Coefficients<float>::makeNotch(getSampleRate(), frequency, Q);
        break;

    case Peak:
        *filter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), frequency, Q, juce::Decibels::decibelsToGain(gain));
        break;

    case LowShelf:
        *filter.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(getSampleRate(), frequency, Q, juce::Decibels::decibelsToGain(gain));
        break;

    case HighShelf:
        *filter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(getSampleRate(), frequency, Q, juce::Decibels::decibelsToGain(gain));
        break;

    default:
        break;
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SingleBandEQ();
}
