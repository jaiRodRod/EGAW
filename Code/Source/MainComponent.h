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

    juce::ValueTree fileRestoreProjectData;
    juce::ValueTree projectData;
    GlobalPlayhead globalPlayhead;

    AudioSystemBus audioSystemBus;
    UserInterfaceManager userInterfaceManager;
    ProjectFileManager projectFileManager;



    //Para la exportacion de audio
    juce::AudioFormatManager formatManager;

    std::unique_ptr<juce::FileChooser> fileChooser;

    std::unique_ptr<RenderThread> renderThread;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
