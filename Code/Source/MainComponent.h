#pragma once

#include <JuceHeader.h>

#include "AudioSystemBus.h"
#include "UserInterfaceManager.h"
#include "ProjectFileManager.h"
#include "DisplaySingleton.h"
#include "SignalManagerUI.h"
#include "ProgressWindowUI.h"
#include "RenderThread.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent, public juce::Value::Listener
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
    void valueChanged(juce::Value& value) override;
    void startRendering();


private:

    /*
    juce::TextButton loadButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::TextButton saveButton;
    juce::TextButton loadFileButton;
    void loadButtonClicked();
    void saveButtonClicked();
    void loadFileButtonClicked();
    void playButtonClicked();
    void stopButtonClicked();
    */

    juce::ValueTree fileRestoreProjectData;
    juce::ValueTree projectData;
    AudioSystemBus audioSystemBus;
    UserInterfaceManager userInterfaceManager;
    ProjectFileManager projectFileManager;


    //Para la exportacion de audio
    juce::AudioFormatManager formatManager;
    //juce::AudioTransportSource audioSource;
    //std::vector

    std::unique_ptr<juce::FileChooser> fileChooser;

    std::unique_ptr<RenderThread> renderThread;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
