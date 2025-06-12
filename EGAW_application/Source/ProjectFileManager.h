/*
  ==============================================================================

    ProjectFileManager.h
    Created: 21 Feb 2025 10:57:31am
    Author:  jaezro

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SignalManagerUI.h"
#include "SoloControlSingleton.h"

class ProjectFileManager : public juce::MessageListener
{
public:

    ProjectFileManager(juce::ValueTree&, juce::ValueTree&);
    ~ProjectFileManager();

	void handleMessage(const juce::Message& message) override;

private:

    void save();
    void saveAs();
    void selectSaveFileInternal();
    void saveFileInternal();

    void loadFile();
    void selectLoadFileInternal();

    void clear();

    juce::ValueTree fileRestoreProjectData;
    juce::ValueTree projectData;

    std::unique_ptr<juce::FileChooser> fileChooser;
    std::unique_ptr<juce::XmlElement> xml;
    juce::File saveFile;
    juce::String saveFilePath;
    juce::String projectName;
};