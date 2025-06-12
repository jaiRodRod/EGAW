#include "MainComponent.h"

#include "RenderThread.h"

//==============================================================================
MainComponent::MainComponent() 
    : projectData("AudioSystemBus")
    , fileRestoreProjectData("RestorerValueTree")
    , globalPlayhead()
    , audioSystemBus(deviceManager.getAudioDeviceSetup(), projectData, fileRestoreProjectData, globalPlayhead)
    , userInterfaceManager(projectData, globalPlayhead.getState(), globalPlayhead)
    , projectFileManager(projectData, fileRestoreProjectData)
{

    tooltipWindow.setMillisecondsBeforeTipAppears(1000);

    SignalManagerUI::getInstance().addListener(this);

    setAudioChannels(0, 2);

    auto currentSetup = deviceManager.getAudioDeviceSetup();

    globalPlayhead.setSampleRate(currentSetup.sampleRate);
    globalPlayhead.setBufferSize(currentSetup.bufferSize);
    juce::Logger::writeToLog("Sample Rate: " + juce::String(currentSetup.sampleRate));
    juce::Logger::writeToLog("Buffer Size: " + juce::String(currentSetup.bufferSize));

    audioSystemBus.setAudioDeviceSetup(deviceManager.getAudioDeviceSetup());

    projectData.setProperty("View", "ROOM", nullptr); //SETEA LA VISTA ACTUAL
    projectData.setProperty("Zoom", playlistThumbnailZoomConstants::x1, nullptr); //SETEA EL ZOOM ACTUAL
    projectData.setProperty("bpm", "90.00", nullptr); //SETEA EL BPM INICIAL

    formatManager.registerBasicFormats();

    setWantsKeyboardFocus(true);
    //grabKeyboardFocus();
    addKeyListener(commandManager.getKeyMappings());
	registerCommands();

	pluginFormatManager.addDefaultFormats();
    scanForPlugins();
    PluginActionStateManager::getInstance().setPluginFormatManager(&pluginFormatManager);
	PluginActionStateManager::getInstance().setPluginList(&pluginList);

    addAndMakeVisible(userInterfaceManager);
    setSize (800, 600);
}

MainComponent::~MainComponent()
{
	SignalManagerUI::getInstance().removeListener(this);
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    audioSystemBus.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    audioSystemBus.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    audioSystemBus.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    userInterfaceManager.paint(g);
}

void MainComponent::resized()
{
    userInterfaceManager.setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);

}

void MainComponent::handleMessage(const juce::Message& message)
{
    if (auto* signalMsg = dynamic_cast<const SignalMessage*>(&message)) {
        auto signal = static_cast<SignalManagerUI::Signal>(signalMsg->getSignalType());
        // Handle signal (already on message thread)...

        switch (signal)
        {
        case SignalManagerUI::Signal::RUN_EXPORT_PROCESS:
            startRendering();
            break;
        default:
            break;
        }
    }
}

void MainComponent::startRendering()
{
    fileChooser = std::make_unique<juce::FileChooser>("Save WAV File",
        juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
        "*.wav");

    fileChooser->launchAsync(juce::FileBrowserComponent::saveMode,
        [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file != juce::File{})
            {
                audioSystemBus.getTransportSource().setPosition(0.0);

                auto setup = deviceManager.getAudioDeviceSetup();
                double sampleRate = setup.sampleRate;
                int bufferSize = setup.bufferSize;

                // Create and start render thread
                renderThread = std::make_unique<RenderThread>(audioSystemBus, file, sampleRate, bufferSize, (double)globalPlayhead.getState().getProperty("realAudioTime"));

                // Show progress window
                auto* progressWindow = new ProgressWindowUI(*renderThread, "Rendering...");
                progressWindow->enterModalState(true, nullptr, true);

                renderThread->startThread();
            }
            fileChooser.reset();
        });
}

void MainComponent::scanForPlugins()
{
    // File to track crashed plugins
    auto deadMansPedalFile = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
        .getChildFile("PluginScanDeadMan.txt");

    for (auto* format : pluginFormatManager.getFormats())
    {
        auto locations = format->getDefaultLocationsToSearch();
        locations.addPath(juce::FileSearchPath("C:\\Program Files\\Common Files\\VST3"));
        locations.addPath(juce::FileSearchPath("C:\\Program Files\\VST3"));
        locations.addPath(juce::FileSearchPath("C:\\Program Files\\Steinberg\\VST3"));

        juce::File parentDir = juce::File::getSpecialLocation(juce::File::currentExecutableFile)
            .getParentDirectory();

        // Access the VST3 folder
        juce::File vst3Folder = parentDir.getChildFile("VST3");

        // Check if it exists and is a directory
        if (vst3Folder.isDirectory())
        {
            locations.add(vst3Folder);
        }
        else
        {
            DBG("VST3 folder not found at: " + vst3Folder.getFullPathName());
        }


        auto scanner = std::make_unique<juce::PluginDirectoryScanner>(
            pluginList,
            *format,
            locations,
            true, // recursive
            deadMansPedalFile
        );

        // Start async scan
        pluginScanners.add(std::move(scanner));
        scanNextPlugin();
    }
}

void MainComponent::scanNextPlugin()
{
    if (pluginScanners.isEmpty())
    {
        // All scans complete
        /*
        juce::MessageManager::callAsync([this]() {
            if (onScanComplete)
                onScanComplete();
            });
        return;
        */
        return;
    }

    auto* scanner = pluginScanners.getFirst();
    juce::String pluginName;

    if (scanner->scanNextFile(true, pluginName))
    {
        // Found a plugin - continue scanning
        scanNextPlugin();
    }
    else
    {
        // This format scan complete
        pluginScanners.remove(0);
        scanNextPlugin();
    }
}

void MainComponent::registerCommands()
{
	commandManager.registerAllCommandsForTarget(this);

    // El espacio acciona play/pause
    commandManager.getKeyMappings()->addKeyPress(playPauseAudio, 
        juce::KeyPress(juce::KeyPress::spaceKey));

	// Ctrl + espacio acciona stop
	commandManager.getKeyMappings()->addKeyPress(stopAudio, 
        juce::KeyPress(juce::KeyPress::spaceKey, juce::ModifierKeys::ctrlModifier, 0));

	// Ctrl + "+" acciona zoom in
	commandManager.getKeyMappings()->addKeyPress(zoomIn,
        juce::KeyPress(static_cast<int>('+'), juce::ModifierKeys::ctrlModifier, '+'));

	// Ctrl + "-" acciona zoom out
	commandManager.getKeyMappings()->addKeyPress(zoomOut,
        juce::KeyPress(static_cast<int>('-'), juce::ModifierKeys::ctrlModifier, '-'));

	//Ctrl + Left Arrow acciona rewind 5 seconds
	commandManager.getKeyMappings()->addKeyPress(rewind5Seconds
        ,juce::KeyPress(juce::KeyPress::leftKey, juce::ModifierKeys::ctrlModifier, 0));

	//Ctrl + Right Arrow acciona advance 5 seconds
	commandManager.getKeyMappings()->addKeyPress(advance5Seconds
		, juce::KeyPress(juce::KeyPress::rightKey, juce::ModifierKeys::ctrlModifier, 0));

    //F1 shows mixer view
	commandManager.getKeyMappings()->addKeyPress(setMixerView
        , juce::KeyPress(juce::KeyPress::F1Key, 0, 0));

	//F2 shows playlist view
	commandManager.getKeyMappings()->addKeyPress(setPlaylistView
		, juce::KeyPress(juce::KeyPress::F2Key, 0, 0));

	//F3 shows listening room view
	commandManager.getKeyMappings()->addKeyPress(setRoomView
		, juce::KeyPress(juce::KeyPress::F3Key, 0, 0));

	commandManager.getKeyMappings()->addKeyPress(loadFile,
		juce::KeyPress(static_cast<int>('l'), juce::ModifierKeys::ctrlModifier, 'l'));

	commandManager.getKeyMappings()->addKeyPress(saveProject,
		juce::KeyPress(static_cast<int>('s'), juce::ModifierKeys::ctrlModifier, 's'));

	commandManager.getKeyMappings()->addKeyPress(saveProjectAs,
		juce::KeyPress(static_cast<int>('s'), juce::ModifierKeys::ctrlModifier | juce::ModifierKeys::shiftModifier, 's'));

	commandManager.getKeyMappings()->addKeyPress(renderAudio,
		juce::KeyPress(static_cast<int>('r'), juce::ModifierKeys::ctrlModifier, 'r'));


	commandManager.getKeyMappings()->addKeyPress(addMixBusChannel,
		juce::KeyPress(static_cast<int>('m'), juce::ModifierKeys::altModifier, 'm'));

	commandManager.getKeyMappings()->addKeyPress(addAudioChannel,
		juce::KeyPress(static_cast<int>('a'), juce::ModifierKeys::altModifier, 'a'));

	commandManager.getKeyMappings()->addKeyPress(addPluginChannel,
		juce::KeyPress(static_cast<int>('p'), juce::ModifierKeys::altModifier, 'p'));
}

juce::ApplicationCommandTarget* MainComponent::getNextCommandTarget()
{
    return nullptr;
}

void MainComponent::getAllCommands(juce::Array<juce::CommandID>& commands)
{
	commands.add(playPauseAudio);
	commands.add(stopAudio);
	commands.add(zoomIn);
	commands.add(zoomOut);
	commands.add(rewind5Seconds);
	commands.add(advance5Seconds);
	commands.add(setMixerView);
	commands.add(setPlaylistView);
	commands.add(setRoomView);
	commands.add(loadFile);
	commands.add(saveProject);
	commands.add(saveProjectAs);
	commands.add(renderAudio);
	commands.add(addMixBusChannel);
	commands.add(addAudioChannel);
	commands.add(addPluginChannel);
}

void MainComponent::getCommandInfo(const juce::CommandID commandID, juce::ApplicationCommandInfo& result)
{
	switch (commandID)
	{
	case playPauseAudio:
		result.setInfo("Play/Pause", "Play or pause the audio", "Transport", 0);
		result.addDefaultKeypress(juce::KeyPress::spaceKey, 0);
		break;
	case stopAudio:
		result.setInfo("Stop", "Stop the audio", "Transport", 0);
		result.addDefaultKeypress(juce::KeyPress::spaceKey, juce::ModifierKeys::ctrlModifier);
		break;
	case zoomIn:
		result.setInfo("Zoom In", "Zoom in the playlist", "View", 0);
		result.addDefaultKeypress(juce::KeyPress::numberPadAdd, juce::ModifierKeys::ctrlModifier);
		break;
	case zoomOut:
		result.setInfo("Zoom Out", "Zoom out the playlist", "View", 0);
		result.addDefaultKeypress(juce::KeyPress::numberPadSubtract, juce::ModifierKeys::ctrlModifier);
		break;
	case rewind5Seconds:
		result.setInfo("Rewind 5 Seconds", "Rewind the playhead 5 seconds", "Transport", 0);
		result.addDefaultKeypress(juce::KeyPress::leftKey, juce::ModifierKeys::ctrlModifier);
		break;
	case advance5Seconds:
		result.setInfo("Advance 5 Seconds", "Advance the playhead 5 seconds", "Transport", 0);
		result.addDefaultKeypress(juce::KeyPress::rightKey, juce::ModifierKeys::ctrlModifier);
		break;
	case setMixerView:
		result.setInfo("Mixer View", "Set the view to mixer", "View", 0);
		result.addDefaultKeypress(juce::KeyPress::F1Key, 0);
		break;
	case setPlaylistView:
		result.setInfo("Playlist View", "Set the view to playlist", "View", 0);
		result.addDefaultKeypress(juce::KeyPress::F2Key, 0);
		break;
	case setRoomView:
		result.setInfo("Listening Room View", "Set the view to listening room", "View", 0);
		result.addDefaultKeypress(juce::KeyPress::F3Key, 0);
		break;
	case loadFile:
		result.setInfo("Load File", "Load an audio file", "File", 0);
		result.addDefaultKeypress(static_cast<int>('l'), juce::ModifierKeys::ctrlModifier);
		break;  
	case saveProject:
		result.setInfo("Save Project", "Save the current project", "File", 0);
		result.addDefaultKeypress(static_cast<int>('s'), juce::ModifierKeys::ctrlModifier);
		break;
	case saveProjectAs:
		result.setInfo("Save Project As", "Save the current project with a new name", "File", 0);
		result.addDefaultKeypress(static_cast<int>('s'), juce::ModifierKeys::ctrlModifier | juce::ModifierKeys::shiftModifier);
		break;
	case renderAudio:
		result.setInfo("Render Audio", "Render the audio to a file", "File", 0);
		result.addDefaultKeypress(static_cast<int>('r'), juce::ModifierKeys::ctrlModifier);
		break;
	case addMixBusChannel:
		result.setInfo("Add Mix Bus Channel", "Add a new mix bus channel", "Channel", 0);
		result.addDefaultKeypress(static_cast<int>('m'), juce::ModifierKeys::altModifier);
		break;
	case addAudioChannel:
		result.setInfo("Add Audio Channel", "Add a new audio channel", "Channel", 0);
		result.addDefaultKeypress(static_cast<int>('a'), juce::ModifierKeys::altModifier);
		break;
	case addPluginChannel:
		result.setInfo("Add Plugin Channel", "Add a new plugin channel", "Channel", 0);
		result.addDefaultKeypress(static_cast<int>('p'), juce::ModifierKeys::altModifier);
		break;
	default:
		break;
	}
}

bool MainComponent::perform(const juce::ApplicationCommandTarget::InvocationInfo& info)
{
    switch (info.commandID)
    {
	case playPauseAudio:
        userInterfaceManager.getFooter().getPlayerControlGrid().playButtonClicked();
		return true;
	case stopAudio:
		userInterfaceManager.getFooter().getPlayerControlGrid().stopButtonClicked();
		return true;
	case zoomIn:
        userInterfaceManager.getFooter().getPlaylistZoomComponent().zoomIn();
		return true;
	case zoomOut:
		userInterfaceManager.getFooter().getPlaylistZoomComponent().zoomOut();
		return true;
	case rewind5Seconds:
		globalPlayhead.substract_seconds(5);
		return true;
	case advance5Seconds:
		globalPlayhead.add_seconds(5);
		return true;
	case setMixerView:
		projectData.setProperty("View", "MIXER", nullptr);
		return true;
	case setPlaylistView:
		projectData.setProperty("View", "PLAYLIST", nullptr);
		return true;
	case setRoomView:
		projectData.setProperty("View", "ROOM", nullptr);
		return true;
	case loadFile:
        SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::LOAD_FILE);
		return true;
	case saveProject:
		SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::SAVE_FILE);
		return true;
	case saveProjectAs:
		SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::SAVE_AS_FILE);
		return true;
	case renderAudio:
		SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::RUN_EXPORT_PROCESS);
		return true;
	case addMixBusChannel:
		SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::ADD_MIX_BUS_CHANNEL);
		return true;
	case addAudioChannel:
		SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::ADD_AUDIO_CHANNEL);
		return true;
	case addPluginChannel:
		SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::ADD_PLUGIN_RACK_CHANNEL);
		return true;
	default:
        break;
    }
	return false;
}
