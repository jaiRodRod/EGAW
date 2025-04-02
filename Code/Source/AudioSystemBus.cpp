/*
  ==============================================================================

    AudioSystemBus.cpp
    Created: 10 Feb 2025 4:45:15pm
    Author:  jaezro

  ==============================================================================
*/

#include "AudioSystemBus.h"

AudioSystemBus::AudioSystemBus(juce::AudioDeviceManager::AudioDeviceSetup& audioDeviceSetupToAttach
    , juce::ValueTree& projectData, juce::ValueTree& fileRestoreProjectData, GlobalPlayhead& globalPlayhead)
    : channelOrder("channelOrder")
    , audioDeviceSetup(audioDeviceSetupToAttach)
    , transportSource()
    , masterBusChannel(juce::Uuid("MasterBusChannel").toString(), juce::ValueTree(juce::Uuid("MasterBusChannel").toString()))
    , projectData(projectData)
    , fileRestoreProjectData(fileRestoreProjectData)
    , globalPlayhead(globalPlayhead)
{

    playing = false;
    globalPlayhead.getState().setProperty("isPlaying", false, nullptr);

    SignalManagerUI::getInstance().addListener(this);
    RoutingActionStateManager::getInstance().addListener(this);

    masterBusChannel.setInternalChannelId(juce::Uuid("MasterBusChannel"));
    transportSource.setSource(&masterBusChannel);

    masterBusChannel.setMixerPosition(channelOrder.getNumChildren());
    channelOrder.appendChild(juce::ValueTree(masterBusChannel.getInternalChannelId()), nullptr);

    projectData.appendChild(channelOrder, nullptr);
    projectData.appendChild(masterBusChannel.getValueTree(), nullptr);
    projectData.addListener(this);

    DBG("AudioDeviceSetup sample rate = " << audioDeviceSetup.sampleRate);

    prepareToPlay(audioDeviceSetup.bufferSize, audioDeviceSetup.sampleRate);
}

AudioSystemBus::~AudioSystemBus()
{
    SignalManagerUI::getInstance().removeListener(this);
    RoutingActionStateManager::getInstance().removeListener(this);
    projectData.removeListener(this);
    
    tempAudioChannel = nullptr;
    delete tempAudioChannel;

    tempMixBusChannel = nullptr;
	delete tempMixBusChannel;

    //delete audioDeviceSetup;
}

void AudioSystemBus::valueTreeChildRemoved(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved)
{
    if (parentTree == projectData)
    {
        bool removed = false;
        juce::String childRemovedUuid = childWhichHasBeenRemoved.getType().toString();
        Channel* channel = getChannelByUUID(childRemovedUuid);

        if (MixBusChannel* mixBusChannel = dynamic_cast<MixBusChannel*>(channel))
        {
            removeMixBusChannel(mixBusChannel, childWhichHasBeenRemoved.getChildWithName("channelsRoutedTo"));
        }
        else if (AudioChannel* audioChannel = dynamic_cast<AudioChannel*>(channel))
        {
            removeAudioChannel(audioChannel, childWhichHasBeenRemoved.getChildWithName("channelsRoutedTo"));
        }

    }
}

void AudioSystemBus::handleMessage(const juce::Message& message)
{
    if (auto* signalMsg = dynamic_cast<const SignalMessage*>(&message)) {
        auto signal = static_cast<SignalManagerUI::Signal>(signalMsg->getSignalType());
        
        switch (signal)
        {
        case SignalManagerUI::Signal::RESTORE_PROJECT_DATA:
            restoreProjectData();
            break;
        case SignalManagerUI::Signal::PLAY_AUDIO:
            playing = true;
            globalPlayhead.setIsPlaying(playing);
            transportSource.start();
            break;
        case SignalManagerUI::Signal::PAUSE_AUDIO:
            playing = false;
            globalPlayhead.setIsPlaying(playing);
            transportSource.stop();
            break;
        case SignalManagerUI::Signal::STOP_AUDIO:
            playing = false;
            globalPlayhead.setIsPlaying(playing);
            transportSource.stop();
            setTransportToBegin();
            prepareToPlay(audioDeviceSetup.bufferSize, audioDeviceSetup.sampleRate);
            break;
        case SignalManagerUI::Signal::ADD_AUDIO_CHANNEL:
            addAudioChannel();
            break;
        case SignalManagerUI::Signal::DO_ADD_AUDIO_CHANNEL:
            doAddAudioChannel();
            SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
            break;
        case SignalManagerUI::Signal::ADD_MIX_BUS_CHANNEL:
            addMixBusChannel();
            break;
        default:
            break;
        }
    }
    else if (const auto* routingMsg = dynamic_cast<const RoutingMessage*>(&message))
    {
        const auto state = static_cast<RoutingActionStateManager::RoutingState>(routingMsg->routingState);

        switch (state)
        {
        case RoutingActionStateManager::RoutingState::COMPLETED_ROUTING:
            doRouting();
            RoutingActionStateManager::getInstance().setOriginChannelUuid(juce::String());
            RoutingActionStateManager::getInstance().setDestinyChannelUuid(juce::String());
            RoutingActionStateManager::getInstance().setState(RoutingActionStateManager::RoutingState::ROUTING_OFF);
            break;
        case RoutingActionStateManager::RoutingState::COMPLETED_REMOVING_ROUTE:
            doRemoveRoute();
            RoutingActionStateManager::getInstance().setOriginChannelUuid(juce::String());
            RoutingActionStateManager::getInstance().setDestinyChannelUuid(juce::String());
            RoutingActionStateManager::getInstance().setState(RoutingActionStateManager::RoutingState::ROUTING_OFF);
        default:
            break;
        }
    }
}

void AudioSystemBus::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    DBG("PREPARE TO PLAY buffer size = " << audioDeviceSetup.bufferSize);
    DBG("PREPARE TO PLAY sample rate = " << audioDeviceSetup.sampleRate);

    //GlobalPlayhead::getInstance()->setSampleRate(sampleRate);

    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    masterBusChannel.prepareToPlay(samplesPerBlockExpected, sampleRate);
    for (auto* channel : audioChannels)
        channel->prepareToPlay(samplesPerBlockExpected, sampleRate);
    for (auto* bus : mixBusChannels)
        bus->prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void AudioSystemBus::releaseResources()
{
    transportSource.releaseResources();
}

void AudioSystemBus::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{

    //DBG("BufferToFill num samples = " << bufferToFill.numSamples);


    //DBG("getNextAudioBlock buffer size = " << audioDeviceSetup.bufferSize);
    //DBG("getNextAudioBlock sample rate = " << audioDeviceSetup.sampleRate);

    bufferToFill.clearActiveBufferRegion();
    transportSource.getNextAudioBlock(bufferToFill);

    if(playing)
        globalPlayhead.add_numSamples(bufferToFill.numSamples);
    //globalPlayhead += bufferToFill.numSamples;
}

void AudioSystemBus::setNextReadPosition(juce::int64 newPosition)
{
    globalPlayhead.setPlayheadPosition(newPosition);
    //globalPlayhead.store(newPosition);
    transportSource.setNextReadPosition(newPosition);
}

juce::int64 AudioSystemBus::getNextReadPosition() const
{
    return transportSource.getNextReadPosition();
}

juce::int64 AudioSystemBus::getTotalLength() const
{
    return transportSource.getTotalLength();
}

bool AudioSystemBus::isLooping() const
{
    return transportSource.isLooping();
}


void AudioSystemBus::addMixBusChannel()
{
    mixBusChannels.add(new MixBusChannel());
    mixBusChannels.getLast()->setMixerPosition(channelOrder.getNumChildren());
    mixBusChannels.getLast()->routeTo(&masterBusChannel, true);

    channelOrder.appendChild(juce::ValueTree(mixBusChannels.getLast()->getInternalChannelId()), nullptr);
    auto mixBusChannelTree = mixBusChannels.getLast()->getValueTree();
    auto nombreMixBusChannel = juce::String("Mix Bus ");
    nombreMixBusChannel.append(juce::String(mixBusChannels.size()), 3);
    mixBusChannelTree.setProperty("Name", nombreMixBusChannel, nullptr);
    projectData.appendChild(mixBusChannelTree, nullptr);

    prepareToPlay(audioDeviceSetup.bufferSize, audioDeviceSetup.sampleRate);

    SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
}

void AudioSystemBus::removeMixBusChannel(MixBusChannel* mixBusChannelToRemove, juce::ValueTree channelsRoutedTo)
{
    //Este metodo se usa para eliminar definitivamente
    //Para desenrutar y enrutar usar sus metodos

    //Si esta en Solo pues se cancela
    //if Solo:         SoloControlSingleton::getInstance()->setSignal(SoloControlSingleton::SoloSignal::GLOBAL_SOLO_OFF);

    if (mixBusChannelToRemove->isSoloOn())
    {
        SoloControlSingleton::getInstance()->setSignal(SoloControlSingleton::SoloSignal::GLOBAL_SOLO_OFF);
    }

    //Eliminar todos los enrutados de todos los routableChannel a los que hayan podido llegar

    for (int i = 0; i < channelsRoutedTo.getNumChildren(); ++i)
    {
        auto routeChannelUuid = channelsRoutedTo.getChild(i).getType().toString();
        if (auto routableChannel = getRoutableChannelByUUID(routeChannelUuid))
        {
            mixBusChannelToRemove->removeRouteTo(routableChannel);
        }
    }

    auto mixBusChannelUuid = mixBusChannelToRemove->getInternalChannelId();
    //Nos saltamos el primer hijo porque es channelOrder
    for (int i = 1; i < projectData.getNumChildren(); ++i)
    {
        auto channelData = projectData.getChild(i);
        auto targetChannelsRoutedTo = channelData.getChildWithName("channelsRoutedTo");
        for (int j = 0; j < targetChannelsRoutedTo.getNumChildren(); ++j)
        {
            if(mixBusChannelUuid == targetChannelsRoutedTo.getChild(j).getType().toString())
            {
                auto channel = getChannelByUUID(channelData.getType().toString());
                channel->removeRouteTo(mixBusChannelToRemove);
            }
        }
    }

    mixBusChannels.removeObject(mixBusChannelToRemove, true);
}

juce::Array<MixBusChannel*> AudioSystemBus::getMixBusChannels()
{
    juce::Array<MixBusChannel*> mixBusChannelsResult;
    for (MixBusChannel* c : mixBusChannels)
    {
        mixBusChannelsResult.add(c);
    }
    return mixBusChannelsResult;
}

void AudioSystemBus::addAudioChannel()
{
    //El procedimiento de hacer un new y despues unirlo al audioChannels puede traer fugas de memoria e inconsistencias
    tempAudioChannel = new AudioChannel(globalPlayhead);
}

void AudioSystemBus::doAddAudioChannel()
{
    audioChannels.add(tempAudioChannel);
    tempAudioChannel->setMixerPosition(channelOrder.getNumChildren());
    tempAudioChannel->routeTo(&masterBusChannel, true);

    globalPlayhead.contestForTimeLength(tempAudioChannel->getTotalLength());

    channelOrder.appendChild(juce::ValueTree(tempAudioChannel->getInternalChannelId()), nullptr);
    projectData.appendChild(tempAudioChannel->getValueTree(), nullptr);

    prepareToPlay(audioDeviceSetup.bufferSize, audioDeviceSetup.sampleRate);
}

void AudioSystemBus::doRouting()
{
    auto originChannelUuid = RoutingActionStateManager::getInstance().getOriginChannelUuid();
    auto destinyChannelUuid = RoutingActionStateManager::getInstance().getDestinyChannelUuid();

    auto originChannel = getChannelByUUID(originChannelUuid);
    auto destinyChannel = getRoutableChannelByUUID(destinyChannelUuid);

    originChannel->routeTo(destinyChannel, true);

    prepareToPlay(audioDeviceSetup.bufferSize, audioDeviceSetup.sampleRate);
}

void AudioSystemBus::doRemoveRoute()
{
    auto originChannelUuid = RoutingActionStateManager::getInstance().getOriginChannelUuid();
    auto destinyChannelUuid = RoutingActionStateManager::getInstance().getDestinyChannelUuid();

    auto originChannel = getChannelByUUID(originChannelUuid);
    auto destinyChannel = getRoutableChannelByUUID(destinyChannelUuid);

    originChannel->removeRouteTo(destinyChannel);

    prepareToPlay(audioDeviceSetup.bufferSize, audioDeviceSetup.sampleRate);
}

void AudioSystemBus::removeAudioChannel(AudioChannel* audioChannelToRemove, juce::ValueTree channelsRoutedTo)
{
    //Si esta en Solo pues se cancela
    //if Solo:         SoloControlSingleton::getInstance()->setSignal(SoloControlSingleton::SoloSignal::GLOBAL_SOLO_OFF);

    if (audioChannelToRemove->isSoloOn())
    {
        SoloControlSingleton::getInstance()->setSignal(SoloControlSingleton::SoloSignal::GLOBAL_SOLO_OFF);
    }

    //Eliminar todos los enrutados de todos los routableChannel a los que hayan podido llegar

    for (int i = 0; i < channelsRoutedTo.getNumChildren(); ++i)
    {
        auto routeChannelUuid = channelsRoutedTo.getChild(i).getType().toString();
        if (auto routableChannel = getRoutableChannelByUUID(routeChannelUuid))
        {
            audioChannelToRemove->removeRouteTo(routableChannel);
        }
    }

    audioChannels.removeObject(audioChannelToRemove, true);
}

juce::Array<AudioChannel*> AudioSystemBus::getAudioChannels()
{
    juce::Array<AudioChannel*> audioChannelsResult;
    for (AudioChannel* c : audioChannels)
    {
        audioChannelsResult.add(c);
    }
    return audioChannelsResult;
}

void AudioSystemBus::routeInMasterBusChannel(Channel* channelToRouteIn)
{
    channelToRouteIn->routeTo(&masterBusChannel, true);
}

void AudioSystemBus::removeRouteInMasterBusChannel(Channel* channelToRemoveRouteIn)
{
    channelToRemoveRouteIn->removeRouteTo(&masterBusChannel);
}  

/// TODO ----------------------------------------------------------------------------------

juce::ValueTree& AudioSystemBus::getProjectData()
{
    return projectData;
}

juce::int64 AudioSystemBus::getTotalSamples()
{
    return masterBusChannel.getTotalLength();
}

void AudioSystemBus::setAudioDeviceSetup(juce::AudioDeviceManager::AudioDeviceSetup newAudioDeviceSetup)
{
    audioDeviceSetup = newAudioDeviceSetup;
    prepareToPlay(audioDeviceSetup.bufferSize, audioDeviceSetup.sampleRate);
}

void AudioSystemBus::processNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    DBG("process buffer size = " << audioDeviceSetup.bufferSize);
    DBG("process sample rate = " << audioDeviceSetup.sampleRate);

    bufferToFill.clearActiveBufferRegion();
    masterBusChannel.getNextAudioBlock(bufferToFill);

    globalPlayhead.add_numSamples(bufferToFill.numSamples);
    //globalPlayhead += bufferToFill.numSamples;
}

void AudioSystemBus::start()
{
    transportSource.start();
}

void AudioSystemBus::setTransportToBegin()
{
    transportSource.setPosition(0);
    globalPlayhead.setPlayheadPosition(0);
    //globalPlayhead.store(0);
}

void AudioSystemBus::restoreProjectData()
{
    clearAudioSystemBusState();

    projectData.setProperty("bpm", fileRestoreProjectData.getProperty("bpm"), nullptr);
    projectData.setProperty("saveFilePath", fileRestoreProjectData.getProperty("saveFilePath"), nullptr);
    projectData.setProperty("projectName", fileRestoreProjectData.getProperty("projectName"), nullptr);
    projectData.setProperty("View", fileRestoreProjectData.getProperty("View"), nullptr); //SETEA LA VISTA ACTUAL


    projectData.appendChild(channelOrder, nullptr);

    juce::ValueTree channelOrderRestore(fileRestoreProjectData.getChildWithName("channelOrder"));


    for (int i = 0; i < channelOrderRestore.getNumChildren(); ++i)
    {
        juce::String uuid = channelOrderRestore.getChild(i).getType().toString();
        if (i == 0)
        {
            //masterBusChannel
            juce::ValueTree channelData = fileRestoreProjectData.getChildWithName(uuid);
            masterBusChannel.setSettings(channelData);
            masterBusChannel.setMixerPosition(channelOrder.getNumChildren());
            channelOrder.appendChild(juce::ValueTree(masterBusChannel.getInternalChannelId()), nullptr);
            projectData.appendChild(masterBusChannel.getValueTree(), nullptr);

        }
        else
        {
            juce::ValueTree channelData = fileRestoreProjectData.getChildWithName(uuid);
            juce::String channelType = channelData.getProperty("Type");
            if (channelType == "MixBusChannel")
            {
                mixBusChannels.add(new MixBusChannel(uuid, channelData));
                mixBusChannels.getLast()->setMixerPosition(channelOrder.getNumChildren());
                channelOrder.appendChild(juce::ValueTree(mixBusChannels.getLast()->getInternalChannelId()), nullptr);
                projectData.appendChild(mixBusChannels.getLast()->getValueTree(), nullptr);

                //mixBusChannel->routeTo(&masterBusChannel, true);
            }
            else if (channelType == "AudioChannel")
            {
                audioChannels.add(new AudioChannel(globalPlayhead, uuid, channelData));
                audioChannels.getLast()->setMixerPosition(i);
                channelOrder.appendChild(juce::ValueTree(audioChannels.getLast()->getInternalChannelId()), nullptr);
                projectData.appendChild(audioChannels.getLast()->getValueTree(), nullptr);

                //tempAudioChannel->routeTo(&masterBusChannel, true);
            }
        }
    }

    //ROUTING
    for (MixBusChannel* mixBusChannel : mixBusChannels)
    {
        juce::ValueTree channelData = fileRestoreProjectData.getChildWithName(mixBusChannel->getInternalChannelId());
        juce::ValueTree channelsRoutedTo = channelData.getChildWithName("channelsRoutedTo");

        for (int i = 0; i < channelsRoutedTo.getNumChildren(); ++i)
        {
            RoutableChannel* destinyChannel = getRoutableChannelByUUID(channelsRoutedTo.getChild(i).getType().toString());

            if (destinyChannel != nullptr)
                mixBusChannel->routeTo(destinyChannel, true);
        }
    }

    for (AudioChannel* audioChannel : audioChannels)
    {
        juce::ValueTree channelData = fileRestoreProjectData.getChildWithName(audioChannel->getInternalChannelId());
        juce::ValueTree channelsRoutedTo = channelData.getChildWithName("channelsRoutedTo");

        for (int i = 0; i < channelsRoutedTo.getNumChildren(); ++i)
        {
            RoutableChannel* destinyChannel = getRoutableChannelByUUID(channelsRoutedTo.getChild(i).getType().toString());

            if (destinyChannel != nullptr)
                audioChannel->routeTo(destinyChannel, true);
        }
    }

    juce::Logger::writeToLog("Proyecto recargado");

    prepareToPlay(audioDeviceSetup.bufferSize, audioDeviceSetup.sampleRate);

    juce::Logger::writeToLog("Done");

    SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::REBUILD_UI);
}

Channel* AudioSystemBus::getChannelByUUID(juce::String uuid)
{
    for (MixBusChannel* channel : mixBusChannels)
    {
        if (channel->getInternalChannelId() == uuid)
        {
            return channel;
        }
    }
    for (AudioChannel* channel : audioChannels)
    {
        if (channel->getInternalChannelId() == uuid)
        {
            return channel;
        }
    }
    //Continuar si hay mas channel
    return nullptr;
}

RoutableChannel* AudioSystemBus::getRoutableChannelByUUID(juce::String uuid)
{
    if (masterBusChannel.getInternalChannelId() == uuid)
    {
        return &masterBusChannel;
    }
    else
    {
        for (MixBusChannel* channel : mixBusChannels)
        {
            if (channel->getInternalChannelId() == uuid)
            {
                return channel;
            }
        }
        //continuar si hay mas routableChannel

        return nullptr;
    }
}

void AudioSystemBus::clearAudioSystemBusState()
{
    masterBusChannel.clearMixBusChannel();
    mixBusChannels.clear();
    audioChannels.clear();
    channelOrder.removeAllChildren(nullptr);
}