/*
  ==============================================================================

    AudioChannel.cpp
    Created: 9 Feb 2025 12:57:09pm
    Author:  jaezro

  ==============================================================================
*/

#include "AudioChannel.h"

AudioChannel::AudioChannel() : settings(Channel::getInternalChannelId())
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

    settings.setProperty("Colour", juce::Colours::grey.toString(), nullptr);

    settings.addListener(this);
    SoloControlSingleton::getInstance()->addListener(this);

    formatManager.registerBasicFormats();
    loadFile();
}

AudioChannel::AudioChannel(juce::String& optionalInternalChannelId, juce::ValueTree& restorerValueTree) 
    : settings(optionalInternalChannelId), Channel(optionalInternalChannelId), currentFile(restorerValueTree.getProperty("FilePath"))
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
                SignalManagerUI::getInstance()->setSignal(SignalManagerUI::Signal::DO_ADD_AUDIO_CHANNEL);
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
        readerSource.reset(new juce::AudioFormatReaderSource(reader, true));  // Crear la fuente de audio
        currentFile = audioFile;  // Almacenar el archivo cargado
        settings.setProperty("FilePath", currentFile.getFullPathName(), nullptr);
        if (!settings.hasProperty("Name"))
        {
            settings.setProperty("Name", currentFile.getFileNameWithoutExtension(), nullptr);
        }
        return true;
    }
    return false;  // No se pudo cargar el archivo
}

//Funciones para que herede correctamente de AudioSource
void AudioChannel::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    if (readerSource)
    {
        readerSource->prepareToPlay(samplesPerBlockExpected, sampleRate);
        DBG("Audio Channel PrepareToPlay sampleRate=" << sampleRate);
    }
    isPrepared = true;
}

void AudioChannel::releaseResources()
{
    if (readerSource)
        readerSource->releaseResources();
    isPrepared = false;
}

void AudioChannel::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (isPrepared && readerSource)
    {
        readerSource->getNextAudioBlock(bufferToFill);

        juce::dsp::AudioBlock<float> block(*bufferToFill.buffer);

        DBG("Channel DATA ==============================");
        DBG("Channel mute: " << (channelMute ? "true" : "false"));
        DBG("Global solo: " << (isGlobalSoloActive ? "true" : "false"));
        DBG("Channel solo: " << (channelSolo ? "true" : "false"));

        if ((!channelMute) && ((!isGlobalSoloActive) || (isGlobalSoloActive && channelSolo)))
        {
            DBG("IN AUDIO CHANNEL");
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
        readerSource->setNextReadPosition(newPosition);
    }
}

juce::int64 AudioChannel::getNextReadPosition() const
{
    return readerSource ? readerSource->getNextReadPosition() : 0;
}

juce::int64 AudioChannel::getTotalLength() const
{
    return readerSource ? readerSource->getTotalLength() : 0;
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
