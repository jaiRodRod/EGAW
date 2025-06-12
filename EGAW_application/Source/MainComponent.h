#pragma once

#include <JuceHeader.h>
#include "ZoomConstants.h"

#include "AudioSystemBus.h"
#include "UserInterfaceManager.h"
#include "ProjectFileManager.h"
#include "DisplaySingleton.h"
#include "SignalManagerUI.h"
#include "ProgressWindowUI.h"
#include "RenderThread.h"
#include "GlobalPlayhead.h"
#include "PluginActionStateManager.h"

//==============================================================================
class MainComponent : public juce::AudioAppComponent, public juce::MessageListener,
	public juce::ApplicationCommandTarget
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;


    //For rendering
	void handleMessage(const juce::Message& message) override;
    void startRendering();


private:

	void scanForPlugins();
	void scanNextPlugin();

    void registerCommands();
	juce::ApplicationCommandTarget* getNextCommandTarget() override;
	void getAllCommands (juce::Array<juce::CommandID>& commands) override;
	void getCommandInfo (const juce::CommandID commandID, juce::ApplicationCommandInfo& result) override;
	bool perform (const juce::ApplicationCommandTarget::InvocationInfo& info) override;



    juce::TooltipWindow tooltipWindow;

    juce::ValueTree fileRestoreProjectData;
    juce::ValueTree projectData;
    GlobalPlayhead globalPlayhead;

    AudioSystemBus audioSystemBus;
    UserInterfaceManager userInterfaceManager;
    ProjectFileManager projectFileManager;

	juce::AudioPluginFormatManager pluginFormatManager;
	juce::KnownPluginList pluginList;
	juce::OwnedArray<juce::PluginDirectoryScanner> pluginScanners;
	//bool onScanComplete = false;

    //Para la exportacion de audio
    juce::AudioFormatManager formatManager;

    std::unique_ptr<juce::FileChooser> fileChooser;

    std::unique_ptr<RenderThread> renderThread;


    enum CommandIDs {
        playPauseAudio = 1,
		stopAudio,
		zoomIn,
		zoomOut,
		rewind5Seconds,
		advance5Seconds,
        setMixerView,
		setPlaylistView,
		setRoomView,
		loadFile,
		saveProject,
		saveProjectAs,
		renderAudio,
        addMixBusChannel,
		addAudioChannel,
		addPluginChannel,
    };

	juce::ApplicationCommandManager commandManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
