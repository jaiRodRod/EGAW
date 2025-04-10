/*
  ==============================================================================

    AudioChannel.cpp
    Created: 9 Feb 2025 12:57:09pm
    Author:  jaezro

  ==============================================================================
*/

#include "AudioChannel.h"

AudioChannel::AudioChannel(GlobalPlayhead& globalPlayhead) : globalPlayhead(globalPlayhead), settings(Channel::getInternalChannelId())
{
    gain.setGainDecibels(0.0f);
    panValue = 0.0f;
    pan.setPan(panValue);
    channelMute = false;
    channelSolo = false;
    isGlobalSoloActive = SoloControlSingleton::getInstance()->checkIsGlobalSoloActive();

    settings.setProperty("Type", "AudioChannel", nullptr);
    settings.setProperty("Gain", gain.getGainDecibels(), nullptr);
    settings.setProperty("Pan", panValue, nullptr);
    settings.setProperty("Mute", channelMute, nullptr);
    settings.setProperty("Solo", channelSolo, nullptr);
    settings.setProperty("StartTime", 0.0f, nullptr);

    settings.setProperty("Colour", juce::Colours::grey.toString(), nullptr);

    settings.addListener(this);
    SoloControlSingleton::getInstance()->addListener(this);

    formatManager.registerBasicFormats();
    loadFile();
}

AudioChannel::AudioChannel(GlobalPlayhead& globalPlayhead, juce::String& optionalInternalChannelId, juce::ValueTree& restorerValueTree) 
    : globalPlayhead(globalPlayhead), settings(optionalInternalChannelId), Channel(optionalInternalChannelId), currentFile(restorerValueTree.getProperty("FilePath"))
{
    gain.setGainDecibels((float) restorerValueTree.getProperty("Gain"));
    panValue = (float) restorerValueTree.getProperty("Pan");
    pan.setPan(panValue);
    channelMute = (bool)restorerValueTree.getProperty("Mute");
    channelSolo = (bool)restorerValueTree.getProperty("Solo");
    isGlobalSoloActive = SoloControlSingleton::getInstance()->checkIsGlobalSoloActive();

    settings.setProperty("Type", "AudioChannel", nullptr);
    settings.setProperty("Gain", gain.getGainDecibels(), nullptr);
    settings.setProperty("Pan", panValue, nullptr);
    settings.setProperty("Mute", channelMute, nullptr);
    settings.setProperty("Solo", channelSolo, nullptr);

    settings.setProperty("StartTime", restorerValueTree.getProperty("StartTime"), nullptr);
    setStartTime(settings.getProperty("StartTime"));

    settings.setProperty("AudioFileStart", restorerValueTree.getProperty("AudioFileStart"), nullptr);
    settings.setProperty("AudioFileEnd", restorerValueTree.getProperty("AudioFileEnd"), nullptr);
    settings.setProperty("AudioFileLength", restorerValueTree.getProperty("AudioFileLength"), nullptr);

    settings.setProperty("Colour", restorerValueTree.getProperty("Colour"), nullptr);
    settings.setProperty("Name", restorerValueTree.getProperty("Name"), nullptr);

    settings.addListener(this);
    SoloControlSingleton::getInstance()->addListener(this);

    if (channelSolo)
    {
        SoloControlSingleton::getInstance()->setSignal(SoloControlSingleton::SoloSignal::GLOBAL_SOLO_ON);
    }

    formatManager.registerBasicFormats();
    if (currentFile.exists())
    {
        loadFileInternal(currentFile);
    }
    else
    {
        //Tratamiento del mensaje para recuperar un archivo que no existe mediante UI y decision del usuario
        loadFile();
    }
}

AudioChannel::~AudioChannel()
{
    settings.removeListener(this);
    SoloControlSingleton::getInstance()->removeListener(this);
    releaseResources();
}

void AudioChannel::loadFile()
{
    fileChooser = std::make_unique<juce::FileChooser>(
        "Selecciona un archivo de audio",
        juce::File::getSpecialLocation(juce::File::userHomeDirectory),
        "*.wav;*.aiff;*.mp3"
    );

    fileChooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
        [this](const juce::FileChooser& chooser)
        {
            auto file = chooser.getResult();
            if (file.existsAsFile())
            {
                loadFileInternal(file);
                SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::DO_ADD_AUDIO_CHANNEL);
            }
            fileChooser.reset();
        });
}

bool AudioChannel::isFileLoaded() const
{
    return readerSource != nullptr;
}

juce::File AudioChannel::getLoadedFile() const
{
    return currentFile;
}

bool AudioChannel::loadFileInternal(const juce::File& audioFile)
{
    auto* reader = formatManager.createReaderFor(audioFile);
    if (reader)
    {
        fileSampleRate = reader->sampleRate;

        readerSource.reset(new juce::AudioFormatReaderSource(reader, true));  // Crear la fuente de audio
        resampler.reset(new juce::ResamplingAudioSource(readerSource.get(), false, reader->numChannels));
        resampler->setResamplingRatio(fileSampleRate / sampleRate);

        currentFile = audioFile;  // Almacenar el archivo cargado
        settings.setProperty("FilePath", currentFile.getFullPathName(), nullptr);
        if (!settings.hasProperty("Name"))
        {
            settings.setProperty("Name", currentFile.getFileNameWithoutExtension(), nullptr);
        }
        if (!settings.hasProperty("AudioFileStart"))
        {
			settings.setProperty("AudioFileStart", 0.0f, nullptr);
        }
		if (!settings.hasProperty("AudioFileEnd"))
		{
			settings.setProperty("AudioFileEnd", (double)reader->lengthInSamples / fileSampleRate, nullptr);
		}
        if (!settings.hasProperty("AudioFileLength"))
        {
            settings.setProperty("AudioFileLength", (double)reader->lengthInSamples / fileSampleRate, nullptr);
        }

        return true;
    }
    return false;  // No se pudo cargar el archivo
}

//Funciones para que herede correctamente de AudioSource
void AudioChannel::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    if (resampler)
    {
        this->sampleRate = sampleRate;
        //readerSource->prepareToPlay(samplesPerBlockExpected, sampleRate);
        resampler->prepareToPlay(samplesPerBlockExpected, sampleRate);
        DBG("Audio Channel PrepareToPlay sampleRate=" << sampleRate);
    }
    isPrepared = true;
}

void AudioChannel::releaseResources()
{
    if (readerSource)
        readerSource->releaseResources();

    if (resampler)
        resampler->releaseResources();

    isPrepared = false;
}

void AudioChannel::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    const juce::int64 globalPosition = globalPlayhead.getPlayheadPosition();
	const double audioFileStartSamples = (double)settings.getProperty("AudioFileStart") * sampleRate;
	const double audioFileEndSamples = (double)settings.getProperty("AudioFileEnd") * sampleRate;

    if (isPrepared 
        && readerSource 
        && globalPosition >= startSample.load() 
        && ((globalPosition - startSample.load() + audioFileStartSamples) <= audioFileEndSamples)
        )
    {
        const juce::int64 startInSource = globalPosition - startSample.load() + audioFileStartSamples;

        //readerSource->setNextReadPosition(startInSource);
        //readerSource->getNextAudioBlock(bufferToFill);
        setNextReadPosition(startInSource);
        resampler->getNextAudioBlock(bufferToFill);

        //DBG("Channel DATA ==============================");
        //DBG("FILE SAMPLE RATE: " << fileSampleRate);
        //DBG("SAMPLE RATE: " << sampleRate);

        juce::dsp::AudioBlock<float> block(*bufferToFill.buffer);

        //DBG("Channel DATA ==============================");
        //DBG("Channel mute: " << (channelMute ? "true" : "false"));
        //DBG("Global solo: " << (isGlobalSoloActive ? "true" : "false"));
        //DBG("Channel solo: " << (channelSolo ? "true" : "false"));

        if ((!channelMute) && ((!isGlobalSoloActive) || (isGlobalSoloActive && channelSolo)))
        {
            //DBG("IN AUDIO CHANNEL");
            //DBG("Audio Channel bufferToFill numSamples=" << bufferToFill.numSamples);
            gain.process(juce::dsp::ProcessContextReplacing<float>(block));
            pan.process(juce::dsp::ProcessContextReplacing<float>(block));
        }
        else
        {
            gain.setGainLinear(0.0f);
            gain.process(juce::dsp::ProcessContextReplacing<float>(block));
        }
    }
    else
    {
        bufferToFill.clearActiveBufferRegion();
    }
}

//Funciones para que herede correctamente de PositionableAudioSource
void AudioChannel::setNextReadPosition(juce::int64 newPosition)
{
    if (readerSource)
    {
        double originalPos = newPosition * (fileSampleRate / sampleRate);
        readerSource->setNextReadPosition(static_cast<juce::int64>(originalPos));
    }
}

juce::int64 AudioChannel::getNextReadPosition() const
{
    //return readerSource ? readerSource->getNextReadPosition() : 0;

    if (readerSource)
    {
        double currentPos = readerSource->getNextReadPosition();
        return static_cast<juce::int64>(currentPos * (sampleRate / fileSampleRate));
    }
    return 0;
}

juce::int64 AudioChannel::getTotalLength() const
{
    //return readerSource ? readerSource->getTotalLength() : 0;
    if (readerSource)
    {
        return static_cast<juce::int64>(
            readerSource->getTotalLength() * (sampleRate / fileSampleRate)
        );
    }
    return 0;
}

bool AudioChannel::isLooping() const
{
    return readerSource ? readerSource->isLooping() : false;
}

void AudioChannel::setLooping(bool shouldLoop)
{
    if (readerSource)
        readerSource->setLooping(shouldLoop);
}

void AudioChannel::setStartTime(double seconds)
{
    startSample.store(static_cast<juce::int64>(seconds * sampleRate));
	globalPlayhead.contestForTimeLength(startSample.load() + getTotalLength());
}

juce::ValueTree& AudioChannel::getValueTree()
{
    settings.setProperty("mixerPosition", Channel::getMixerPosition(), nullptr);
    if (!settings.getChildWithName("channelsRoutedTo").isValid())
        settings.appendChild(Channel::getValueTree(), nullptr);
    return this->settings;
}

void AudioChannel::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
    if (settings == treeWhosePropertyHasChanged)
    {
        if (property.toString() == "Mute")
        {
            setMute ((bool)settings.getProperty(property));
        }
        if (property.toString() == "Solo")
        {
            setSolo ((bool)settings.getProperty(property));
        }
        if (property.toString() == "Gain")
        {
            gain.setGainDecibels((float)settings.getProperty(property));
        }
        if (property.toString() == "Pan")
        {
            panValue = (float)settings.getProperty(property);
            pan.setPan(panValue);
        }
        if (property.toString() == "StartTime")
        {
            auto startTime = (float)settings.getProperty(property);
			setStartTime(startTime);
        }
    }
}

void AudioChannel::valueChanged(juce::Value& value)
{
    auto SoloSignal = SoloControlSingleton::getInstance()->getCurrentSignal();

    switch (SoloSignal)
    {
    case SoloControlSingleton::SoloSignal::NULL_SIGNAL:
        isGlobalSoloActive = SoloControlSingleton::getInstance()->checkIsGlobalSoloActive();
        setSoloMute();
        break;
    default:
        break;
    }
}

void AudioChannel::setMute(bool muteValue)
{
    if (muteValue == true)
    {
        // If the channel is muted, set the gain to zero
        gain.setGainLinear(0.0f);
    }
    else
    {
        gain.setGainDecibels(settings.getProperty("Gain"));
    }

    channelMute = muteValue;
}

void AudioChannel::setSolo(bool soloValue)
{
    if (soloValue == true)
    {
        SoloControlSingleton::getInstance()->setSignal(SoloControlSingleton::SoloSignal::GLOBAL_SOLO_ON);
    }
    else
    {
        SoloControlSingleton::getInstance()->setSignal(SoloControlSingleton::SoloSignal::GLOBAL_SOLO_OFF);
    }

    channelSolo = soloValue;
}

void AudioChannel::setSoloMute()
{
    if ((!isGlobalSoloActive) || (channelSolo && isGlobalSoloActive))
    {
        gain.setGainDecibels(settings.getProperty("Gain"));
    }
    else
    {
        gain.setGainLinear(0.0f);
    }
}
