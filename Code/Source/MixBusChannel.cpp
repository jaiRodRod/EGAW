/*
  ==============================================================================

    MixBusChannel.cpp
    Created: 10 Feb 2025 11:28:15am
    Author:  jaezro

  ==============================================================================
*/

#include "MixBusChannel.h"
#include "Channel.h"

MixBusChannel::MixBusChannel() : settings(Channel::getInternalChannelId())
{
    gain.setGainDecibels(0.0f);
    panValue = 0.0f;
    pan.setPan(panValue);
    channelMute = false;
    channelSolo = false;
    isGlobalSoloActive = SoloControlSingleton::getInstance()->checkIsGlobalSoloActive();

    settings.setProperty("Type", "MixBusChannel", nullptr);
    settings.setProperty("Gain", gain.getGainDecibels(), nullptr);
    settings.setProperty("Pan", panValue, nullptr);
    settings.setProperty("Mute", channelMute, nullptr);
    settings.setProperty("Solo", channelSolo, nullptr);

    settings.setProperty("Colour", juce::Colours::brown.toString(), nullptr);

    settings.addListener(this);
    SoloControlSingleton::getInstance()->addListener(this);

    // Inicialización del mixerSource
}

MixBusChannel::MixBusChannel(juce::String optionalInternalChannelId, juce::ValueTree& restorerValueTree)
    : settings(optionalInternalChannelId), RoutableChannel(optionalInternalChannelId)
{
    gain.setGainDecibels((float)restorerValueTree.getProperty("Gain"));
    panValue = (float)restorerValueTree.getProperty("Pan");
    pan.setPan(panValue);
    channelMute = (bool)restorerValueTree.getProperty("Mute");
    channelSolo = (bool)restorerValueTree.getProperty("Solo");
    isGlobalSoloActive = SoloControlSingleton::getInstance()->checkIsGlobalSoloActive();

    settings.setProperty("Type", "MixBusChannel", nullptr);
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
}

MixBusChannel::~MixBusChannel()
{
    // Liberar recursos si es necesario
    settings.removeListener(this);
    SoloControlSingleton::getInstance()->removeListener(this);

}

void MixBusChannel::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    //DBG("Mix Bus Channel PrepareToPlay sampleRate=" << sampleRate);
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MixBusChannel::releaseResources()
{
    mixerSource.releaseResources();
}

void MixBusChannel::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    //DBG("ENTER AUDIO BLOCK MIX BUS");
    if (RoutableChannel::getChanelsRoutedIn().size() > 0)
    {
        //DBG("GETTING NEXT AUDIO BLOCK MIX BUS");
        mixerSource.getNextAudioBlock(bufferToFill);
        juce::dsp::AudioBlock<float> block(*bufferToFill.buffer);

        if ((!channelMute) && ((!isGlobalSoloActive) || (isGlobalSoloActive && channelSolo)))
        {
            DBG("IN MIX BUS");
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

void MixBusChannel::setNextReadPosition(juce::int64 newPosition)
{

    ///Todo: Revisar metodo y comprobar que vaya todo bien

    nextReadPosition = newPosition;

    for (Channel* channel : getChanelsRoutedIn())
    {
        channel->setNextReadPosition(newPosition);
    }
}

juce::int64 MixBusChannel::getNextReadPosition() const
{
    ///Todo: Revisar metodo y comprobar que vaya todo bien

    return nextReadPosition;
}

juce::int64 MixBusChannel::getTotalLength() const
{
    ///Todo: Revisar metodo y comprobar que vaya todo bien

    // Si no tienes una longitud definida, devuelve 0 o un valor que indique "longitud desconocida"
    return getMaxLengthFromChannels();
}

juce::int64 MixBusChannel::getMaxLengthFromChannels() const
{
    juce::int64 maxLength = 0;
    for (Channel* channel : getChanelsRoutedIn())
    {
        auto length = channel->getTotalLength();
        if (length > maxLength)
        {
            maxLength = length;
        }
    }
    return maxLength;
}

bool MixBusChannel::isLooping() const
{
    ///Todo: Revisar metodo y comprobar que vaya todo bien

    for (Channel* channel : getChanelsRoutedIn())
    {
        if (channel->isLooping())
        {
            return true;
        }
    }
    return false;
}

void MixBusChannel::routeIn(Channel* channelIn)
{
    mixerSource.addInputSource(channelIn, false);
    RoutableChannel::routeIn(channelIn);
}

void MixBusChannel::removeRouteIn(Channel* channelIn)
{
    mixerSource.removeInputSource(channelIn);
    RoutableChannel::removeRouteIn(channelIn);
}

void MixBusChannel::clearMixBusChannel()
{
    nextReadPosition = 0;

    for (Channel* c : getChanelsRoutedIn())
    {
        c->removeRouteTo(this);
    }

    mixerSource.removeAllInputs();
    settings.removeAllChildren(nullptr);
    settings.removeAllProperties(nullptr);
}

void MixBusChannel::setSettings(juce::ValueTree settingsValueTree)
{
    //Only used in master

    settings.removeListener(this);

    gain.setGainDecibels((float)settingsValueTree.getProperty("Gain"));
    panValue = (float)settingsValueTree.getProperty("Pan");
    pan.setPan(panValue);
    channelMute = (bool)settingsValueTree.getProperty("Mute");
    channelSolo = (bool)settingsValueTree.getProperty("Solo");
    isGlobalSoloActive = SoloControlSingleton::getInstance()->checkIsGlobalSoloActive();

    settings.setProperty("Type", "MixBusChannel", nullptr);
    settings.setProperty("Gain", gain.getGainDecibels(), nullptr);
    settings.setProperty("Pan", panValue, nullptr);
    settings.setProperty("Mute", channelMute, nullptr);
    settings.setProperty("Solo", channelSolo, nullptr);

    settings.addListener(this);
    SoloControlSingleton::getInstance()->addListener(this);

    if (channelSolo)
    {
        SoloControlSingleton::getInstance()->setSignal(SoloControlSingleton::SoloSignal::GLOBAL_SOLO_ON);
    }
}

void MixBusChannel::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
    if (settings == treeWhosePropertyHasChanged)
    {
        if(property.toString() == "Mute")
        {
            setMute ((bool) settings.getProperty(property));
        }
        if (property.toString() == "Solo")
        {
            setSolo ((bool)settings.getProperty(property));
        }
        if (property.toString() == "Gain")
        {
            gain.setGainDecibels((float) settings.getProperty(property));
        }
        if (property.toString() == "Pan")
        {
            panValue = (float)settings.getProperty(property);
            pan.setPan(panValue);
        }
    }
}

void MixBusChannel::valueChanged(juce::Value& value)
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

void MixBusChannel::setMute(bool muteValue)
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

void MixBusChannel::setSolo(bool soloValue)
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

void MixBusChannel::setSoloMute()
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

juce::ValueTree& MixBusChannel::getValueTree()
{
    settings.setProperty("mixerPosition", Channel::getMixerPosition(), nullptr);
    if (!settings.getChildWithName("channelsRoutedTo").isValid())
        settings.appendChild(Channel::getValueTree(), nullptr);
    return settings;
}
