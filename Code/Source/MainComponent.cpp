#include "MainComponent.h"

#include "RenderThread.h"

//==============================================================================
MainComponent::MainComponent() 
    : projectData("AudioSystemBus")
    , fileRestoreProjectData("RestorerValueTree")
    , globalPlayhead()
    , audioSystemBus(deviceManager.getAudioDeviceSetup(), projectData, fileRestoreProjectData, globalPlayhead)
    , userInterfaceManager(projectData, globalPlayhead.getState())
    , projectFileManager(projectData, fileRestoreProjectData)
{

    SignalManagerUI::getInstance().addListener(this);

    /*
    // ======================================================================
    // Initialize Audio Device
    // ======================================================================
    juce::String error;
    int numInputChannels = 0; // No inputs
    int numOutputChannels = 2; // Stereo output

    juce::AudioDeviceManager::AudioDeviceSetup setup;
    //setup.sampleRate = 44100.0; // Your desired sample rate
    //setup.bufferSize = 512;      // Your desired buffer size

    error = deviceManager.initialise(
        numInputChannels,
        numOutputChannels,
        nullptr,
        true,
        juce::String(),
        &setup
    );
    
    if (error.isNotEmpty()) {
        juce::AlertWindow::showMessageBoxAsync(
            juce::AlertWindow::WarningIcon,
            "Audio Device Error",
            "Could not initialize audio device: " + error
        );
    }
    */

    setAudioChannels(0, 2);

    auto currentSetup = deviceManager.getAudioDeviceSetup();
    globalPlayhead.setSampleRate(currentSetup.sampleRate);
    juce::Logger::writeToLog("Sample Rate: " + juce::String(currentSetup.sampleRate));
    juce::Logger::writeToLog("Buffer Size: " + juce::String(currentSetup.bufferSize));

    audioSystemBus.setAudioDeviceSetup(deviceManager.getAudioDeviceSetup());

    projectData.setProperty("View", "ROOM", nullptr); //SETEA LA VISTA ACTUAL
    projectData.setProperty("Zoom", playlistThumbnailZoomConstants::x1, nullptr); //SETEA EL ZOOM ACTUAL
    projectData.setProperty("bpm", "90.00", nullptr); //SETEA EL BPM INICIAL

    formatManager.registerBasicFormats();
    addAndMakeVisible(userInterfaceManager);
    setSize (800, 600);
}

MainComponent::~MainComponent()
{
	SignalManagerUI::getInstance().removeListener(this);
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    

    //mixBusChannel.prepareToPlay(samplesPerBlockExpected, sampleRate);
    //audioTransportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    //this->samplesPerBlockExpected = samplesPerBlockExpected;
    //this->sampleRate = sampleRate;
    DBG("Prepare to play DONE");

    audioSystemBus.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    

    //audioTransportSource.getNextAudioBlock(bufferToFill);
    //bufferToFill.clearActiveBufferRegion();

    //DBG("Get next audio block ROLLING");
    bufferToFill.clearActiveBufferRegion();
    audioSystemBus.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    
    
    //mixBusChannel.releaseResources();
    //audioTransportSource.releaseResources();
    audioSystemBus.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    userInterfaceManager.paint(g);
}

void MainComponent::resized()
{

    /*
    loadButton.setBounds(10, 10, getWidth() - 20, 30);
    playButton.setBounds(10, 50, getWidth() - 20, 30);
    stopButton.setBounds(10, 90, getWidth() - 20, 30);
    saveButton.setBounds(10, 130, ((getWidth()) / 2) - 10, 30);
    loadFileButton.setBounds(((getWidth()) / 2), 130, ((getWidth()) / 2) - 10, 30);
    */

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
