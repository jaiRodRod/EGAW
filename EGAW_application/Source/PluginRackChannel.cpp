/*
  ==============================================================================

    PluginRackChannel.cpp
    Created: 14 Apr 2025 4:38:16pm
    Author:  jaezro

  ==============================================================================
*/

#include "PluginRackChannel.h"

PluginRackChannel::PluginRackChannel(GlobalPlayhead& globalPlayhead) 
    : settings(Channel::getInternalChannelId())
    , pluginState("plugins")
	, hasBeenReleased(false)
{
    channelMute = false;
    channelSolo = false;
    isGlobalSoloActive = SoloControlSingleton::getInstance()->checkIsGlobalSoloActive();

    settings.setProperty("Type", "PluginRackChannel", nullptr);
    settings.setProperty("Mute", channelMute, nullptr);
    settings.setProperty("Solo", channelSolo, nullptr);

    settings.setProperty("Colour", colourConstants::plugin_channel_base_colour.toString(), nullptr);

	settings.addChild(pluginState, -1, nullptr);

    settings.addListener(this);

	pluginList = std::make_unique<PluginList>(globalPlayhead, pluginState, Channel::getInternalChannelId());

    SoloControlSingleton::getInstance()->addListener(this);
}

PluginRackChannel::PluginRackChannel(GlobalPlayhead& globalPlayhead, juce::String channelId, juce::ValueTree& restorerValueTree)
    : settings(channelId)
    , RoutableChannel(channelId)
	, pluginState("plugins")
    , hasBeenReleased(false)
{
    channelMute = (bool)restorerValueTree.getProperty("Mute");
    channelSolo = (bool)restorerValueTree.getProperty("Solo");
    isGlobalSoloActive = SoloControlSingleton::getInstance()->checkIsGlobalSoloActive();

    settings.setProperty("Type", "PluginRackChannel", nullptr);
    settings.setProperty("Mute", channelMute, nullptr);
    settings.setProperty("Solo", channelSolo, nullptr);
    settings.setProperty("Colour", restorerValueTree.getProperty("Colour"), nullptr);

    settings.setProperty("Name", restorerValueTree.getProperty("Name"), nullptr);

    //pluginState = restorerValueTree.getChildWithName("plugins"); // !!!!!!!!!!!!!!!
	settings.addChild(pluginState, -1, nullptr);

    settings.addListener(this);

    pluginList = std::make_unique<PluginList>(globalPlayhead, pluginState, Channel::getInternalChannelId(), restorerValueTree.getChildWithName("plugins"));

    SoloControlSingleton::getInstance()->addListener(this);

    if (channelSolo)
    {
        SoloControlSingleton::getInstance()->setSignal(SoloControlSingleton::SoloSignal::GLOBAL_SOLO_ON);
    }
}

PluginRackChannel::~PluginRackChannel()
{
    settings.removeListener(this);
    SoloControlSingleton::getInstance()->removeListener(this);
    releaseResources();
}

void PluginRackChannel::prepareToPlay(int samplesPerBlock, double sampleRate)
{
    mixerSource.prepareToPlay(samplesPerBlock, sampleRate);
}

void PluginRackChannel::releaseResources()
{
    if (!hasBeenReleased)
    {
        mixerSource.releaseResources();
        hasBeenReleased = true;
    }
}

void PluginRackChannel::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (RoutableChannel::getChanelsRoutedIn().size() > 0)
    {
        //DBG("GETTING NEXT AUDIO BLOCK MIX BUS");
        mixerSource.getNextAudioBlock(bufferToFill);
        juce::dsp::AudioBlock<float> block(*bufferToFill.buffer);

        if ((!channelMute) && ((!isGlobalSoloActive) || (isGlobalSoloActive && channelSolo)))
        {
            // Playing
			pluginList->processAudioBlock(bufferToFill);
        }
        else
        {
            bufferToFill.clearActiveBufferRegion();
        }
    }
    else
    {
        bufferToFill.clearActiveBufferRegion();
    }
}

void PluginRackChannel::setNextReadPosition(juce::int64 newPosition)
{
    nextReadPosition = newPosition;

    for (Channel* channel : getChanelsRoutedIn())
    {
        channel->setNextReadPosition(newPosition);
    }
}

juce::int64 PluginRackChannel::getNextReadPosition() const
{
    return nextReadPosition;
}

juce::int64 PluginRackChannel::getTotalLength() const
{
    return getMaxLengthFromChannels();
}

bool PluginRackChannel::isLooping() const
{
    for (Channel* channel : getChanelsRoutedIn())
    {
        if (channel->isLooping())
        {
            return true;
        }
    }
    return false;
}

void PluginRackChannel::routeIn(Channel* channelIn)
{
    mixerSource.addInputSource(channelIn, false);
    RoutableChannel::routeIn(channelIn);
}

void PluginRackChannel::removeRouteIn(Channel* channelIn)
{
    mixerSource.removeInputSource(channelIn);
    RoutableChannel::removeRouteIn(channelIn);
}

void PluginRackChannel::clearPluginRackChannel()
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

juce::ValueTree& PluginRackChannel::getSettings()
{
    settings.setProperty("mixerPosition", Channel::getMixerPosition(), nullptr);
    if (!settings.getChildWithName("channelsRoutedTo").isValid())
        settings.appendChild(Channel::getValueTree(), nullptr);
	return settings;
}

void PluginRackChannel::setSettings(juce::ValueTree settingsValueTree)
{
    settings.removeListener(this);

    channelMute = (bool)settingsValueTree.getProperty("Mute");
    channelSolo = (bool)settingsValueTree.getProperty("Solo");
    isGlobalSoloActive = SoloControlSingleton::getInstance()->checkIsGlobalSoloActive();

    settings.setProperty("Type", "MixBusChannel", nullptr);
    settings.setProperty("Mute", channelMute, nullptr);
    settings.setProperty("Solo", channelSolo, nullptr);

    pluginState = settingsValueTree.getChildWithName("plugins"); // !!!!!!!!!!!!!!!
    settings.addChild(pluginState, -1, nullptr);

    settings.addListener(this);
    SoloControlSingleton::getInstance()->addListener(this);

    if (channelSolo)
    {
        SoloControlSingleton::getInstance()->setSignal(SoloControlSingleton::SoloSignal::GLOBAL_SOLO_ON);
    }
}

void PluginRackChannel::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
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
    }
}

void PluginRackChannel::valueChanged(juce::Value& value)
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

juce::int64 PluginRackChannel::getMaxLengthFromChannels() const
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

void PluginRackChannel::setMute(bool muteValue)
{
    channelMute = muteValue;
}

void PluginRackChannel::setSolo(bool soloValue)
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

void PluginRackChannel::setSoloMute()
{
    if ((!isGlobalSoloActive) || (channelSolo && isGlobalSoloActive))
    {
		setMute(false);
    }
    else
    {
		setMute(true);
    }
}
