/*
  ==============================================================================

    ProjectFileManager.cpp
    Created: 21 Feb 2025 10:57:31am
    Author:  jaezro

  ==============================================================================
*/

#include "ProjectFileManager.h"

ProjectFileManager::ProjectFileManager(juce::ValueTree& projectData, juce::ValueTree& fileRestoreProjectData) 
    : projectData(projectData), fileRestoreProjectData(fileRestoreProjectData)
{
    SignalManagerUI::getInstance().addListener(this);
}

ProjectFileManager::~ProjectFileManager()
{
    SignalManagerUI::getInstance().removeListener(this);
}

void ProjectFileManager::handleMessage(const juce::Message& message)
{
    if (auto* signalMsg = dynamic_cast<const SignalMessage*>(&message)) {
        auto signal = static_cast<SignalManagerUI::Signal>(signalMsg->getSignalType());
        // Handle signal (already on message thread)...

        switch (signal)
        {
        case SignalManagerUI::Signal::LOAD_FILE:
            loadFile();
            break;
        case SignalManagerUI::Signal::SAVE_FILE:
            save();
            break;
        case SignalManagerUI::Signal::SAVE_AS_FILE:
            saveAs();
            break;
        default:
            break;
        }
    }
}

void ProjectFileManager::save()
{
    if (saveFilePath != juce::String{} && projectName != juce::String{} && saveFile != juce::File{})
    {
        saveFile = juce::File(saveFilePath);
        if (saveFile.existsAsFile())
        {
            saveFileInternal();
        }
    }
    else
    {
        saveAs();
    }
}

void ProjectFileManager::saveAs()
{
    selectSaveFileInternal();
}

void ProjectFileManager::selectSaveFileInternal()
{
    fileChooser = std::make_unique<juce::FileChooser>("Guardar proyecto como...",
        juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
        "*.egaw");
    fileChooser->launchAsync(juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles,
        [this](const juce::FileChooser& chooser)
        {
            saveFile = chooser.getResult();
            if (saveFile != juce::File{})
            {
                saveFilePath = saveFile.getFullPathName();
                projectName = saveFile.getFileNameWithoutExtension();
                //projectCreateTimeStamp = juce::Time::getCurrentTime();
                saveFile.replaceWithText("");
                saveFileInternal();
            }
            else
            {
                SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::NULL_SIGNAL);
            }
            fileChooser.reset();
        });
}

void ProjectFileManager::saveFileInternal()
{
    projectData.setProperty("saveFilePath", saveFilePath, nullptr);
    projectData.setProperty("projectName", projectName, nullptr);

    //juce::String resultXmlString = projectData.toXmlString();
    //juce::Logger::writeToLog(resultXmlString);

    xml = projectData.createXml();
    if (xml->writeTo(saveFile))
    {
        DBG("Proyecto guardado correctamente en: " + saveFile.getFullPathName());
    }
    else
    {
        DBG("Error al guardar el proyecto.");
    }
}

void ProjectFileManager::loadFile()
{
    selectLoadFileInternal();
}

void ProjectFileManager::selectLoadFileInternal()
{
    fileChooser = std::make_unique<juce::FileChooser>("Abrir proyecto...",
        juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
        "*.egaw");
    fileChooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
        [this](const juce::FileChooser& chooser)
        {
            saveFile = chooser.getResult();
            if (saveFile != juce::File{})
            {
                clear();
                projectData.removeAllChildren(nullptr);
                projectData.removeAllProperties(nullptr);

                saveFilePath = saveFile.getFullPathName();
                projectName = saveFile.getFileNameWithoutExtension();
                
                xml = juce::parseXML(saveFile);
                fileRestoreProjectData.copyPropertiesAndChildrenFrom(juce::ValueTree::fromXml(*(xml.get())), nullptr);

                SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::RESTORE_PROJECT_DATA);
            }
            else
            {
                SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::NULL_SIGNAL);
            }
            fileChooser.reset();
        });
}

void ProjectFileManager::clear()
{
    fileChooser.release();
    xml.release();
    saveFilePath = "";
    projectName = "";
    if(SoloControlSingleton::getInstance())
        SoloControlSingleton::getInstance()->clearSingletonInstance();
}
