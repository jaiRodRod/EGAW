/*
  ==============================================================================

    Plugin.cpp
    Created: 20 Apr 2025 3:07:52pm
    Author:  jaezro

  ==============================================================================
*/

#include "Plugin.h"


Plugin::Plugin(GlobalPlayhead& globalPlayhead, juce::ValueTree& pluginState)
	: globalPlayhead(globalPlayhead)
	, pluginState(pluginState)
	, pluginUuid()
	, pluginSettings(pluginUuid.toString())
	, isUnused(true)
	, pluginFormatManager(*PluginActionStateManager::getInstance().getPluginFormatManager())
{
	pluginState.getChildWithName("pluginOrder").addChild(juce::ValueTree(pluginUuid.toString()), -1, nullptr);

	pluginState.addChild(pluginSettings, -1, nullptr);
	pluginSettings.setProperty("isOccupied", false, nullptr);
	pluginSettings.setProperty("Name", "Plugin", nullptr);
	pluginSettings.setProperty("isBypassed", false, nullptr);
	pluginSettings.setProperty("pluginPosition", pluginState.getChildWithName("pluginOrder").getNumChildren(), nullptr);

	PluginActionStateManager::getInstance().addListener(this);
}

Plugin::Plugin(GlobalPlayhead& globalPlayhead, juce::ValueTree& pluginState, juce::ValueTree pluginStateRestorerValueTree, juce::String restorerPluginUuid)
	: globalPlayhead(globalPlayhead)
	, pluginState(pluginState)
	, pluginUuid(restorerPluginUuid)
	, pluginSettings(restorerPluginUuid)
	, isUnused(true)
	, pluginFormatManager(*PluginActionStateManager::getInstance().getPluginFormatManager())
{
	pluginState.getChildWithName("pluginOrder").addChild(juce::ValueTree(pluginUuid.toString()), -1, nullptr);

	pluginState.addChild(pluginSettings, -1, nullptr);
	pluginSettings.setProperty("isOccupied", pluginStateRestorerValueTree.getChildWithName(restorerPluginUuid).getProperty("isOccupied"), nullptr);
	pluginSettings.setProperty("Name", pluginStateRestorerValueTree.getChildWithName(restorerPluginUuid).getProperty("Name"), nullptr);
	pluginSettings.setProperty("isBypassed", pluginStateRestorerValueTree.getChildWithName(restorerPluginUuid).getProperty("isBypassed"), nullptr);
	pluginSettings.setProperty("pluginPosition", pluginStateRestorerValueTree.getChildWithName(restorerPluginUuid).getProperty("pluginPosition"), nullptr);

	auto isOccupied = (bool)pluginStateRestorerValueTree.getChildWithName(restorerPluginUuid).getProperty("isOccupied");
	if (isOccupied)
	{
		isUnused = false;
		auto pluginDescriptionStringXML = pluginStateRestorerValueTree.getChildWithName(restorerPluginUuid).getChildWithName("pluginDescription").getProperty("descriptionXML").toString();
		std::unique_ptr<juce::XmlElement> pluginDescriptionXML(juce::XmlDocument::parse(pluginDescriptionStringXML));
		auto pluginDescription = std::make_unique<juce::PluginDescription>();
		pluginDescription->loadFromXml(*pluginDescriptionXML);
		reloadPlugin(*pluginDescription, pluginStateRestorerValueTree);
	}

	PluginActionStateManager::getInstance().addListener(this);
}

Plugin::~Plugin()
{
	PluginActionStateManager::getInstance().removeListener(this);
	/*
	pluginInstance->editorBeingDeleted(pluginInstance->getActiveEditor());
	if (pluginUI != nullptr && pluginUI->isVisible())
	{
		pluginUI->setVisible(false);
		pluginUI.reset();
	}
	pluginInstance.reset();
	*/
}

void Plugin::handleMessage(const juce::Message& message)
{
	if (const auto* pluginMsg = dynamic_cast<const PluginMessage*>(&message))
	{
		if (PluginActionStateManager::getInstance().getPluginUuid() == pluginUuid.toString())
		{
			const auto state = static_cast<PluginActionStateManager::PluginSignal>(pluginMsg->pluginSignal);

			switch (state)
			{
			case PluginActionStateManager::PluginSignal::DO_ADD_PLUGIN:
				doAddPlugin();
				break;
			case PluginActionStateManager::PluginSignal::OPEN_PLUGIN:
				doOpenPlugin();
				break;
			case PluginActionStateManager::PluginSignal::REMOVE_PLUGIN:
				doRemovePlugin();
				break;
			case PluginActionStateManager::PluginSignal::CLOSE_PLUGIN:
				doClosePlugin();
				break;
			default:
				break;
			}
		}		
	}
}

void Plugin::prepareToPlay(int samplesPerBlock, double sampleRate)
{
	if (pluginInstance != nullptr)
	{
		pluginInstance->prepareToPlay(sampleRate, samplesPerBlock);
	}
}

void Plugin::releaseResources()
{
	if (pluginInstance != nullptr)
    {
		pluginInstance->releaseResources();
    }
}

void Plugin::processAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	if (pluginInstance != nullptr)
	{
		// Create a temporary buffer that the plugin can process
		juce::AudioBuffer<float> pluginBuffer(bufferToFill.buffer->getArrayOfWritePointers(),
			bufferToFill.buffer->getNumChannels(),
			bufferToFill.startSample,
			bufferToFill.numSamples);

		// Create an empty MIDI buffer (or pass through real MIDI if needed)
		juce::MidiBuffer midiBuffer;
		midiBuffer.clear();

		// Process the buffer through the plugin
		pluginInstance->processBlock(pluginBuffer, midiBuffer);
		
		// The processed audio is now in bufferToFill.buffer
	}
	else
	{
		// Handle case when no plugin is loaded (maybe bypass or silence)
		// bufferToFill.clearActiveBufferRegion();
	}
}

void Plugin::loadPlugin(const juce::PluginDescription& description)
{
	pluginInstance.reset();

	auto onPluginLoaded = [this](std::unique_ptr<juce::AudioPluginInstance> instance, const juce::String& error)
		{
			if (instance != nullptr)
			{
				auto sampleRate = globalPlayhead.getSampleRate();
				auto blockSize = globalPlayhead.getBufferSize();

				pluginInstance = std::move(instance);

				// Set up the plugin
				pluginInstance->setPlayConfigDetails(
					2,
					2,
					sampleRate,
					blockSize);

				pluginInstance->prepareToPlay(sampleRate, blockSize);

				// Notify any listeners that the plugin has changed
				// sendChangeMessage();

				doSavePlugin();
			}
			else if (error.isNotEmpty())
			{
				DBG("Failed to load plugin: " + error);
			}
		};

	auto sampleRate = globalPlayhead.getSampleRate();
	auto blockSize = globalPlayhead.getBufferSize();

	pluginFormatManager.createPluginInstanceAsync(description,
		sampleRate,
		blockSize,
		onPluginLoaded);
}

void Plugin::reloadPlugin(const juce::PluginDescription& description, juce::ValueTree pluginStateRestorerValueTree)
{
	pluginInstance.reset();

	auto onPluginLoaded = [this, pluginStateRestorerValueTree](std::unique_ptr<juce::AudioPluginInstance> instance, const juce::String& error)
		{
			if (instance != nullptr)
			{
				auto sampleRate = globalPlayhead.getSampleRate();
				auto blockSize = globalPlayhead.getBufferSize();

				pluginInstance = std::move(instance);

				// Set up the plugin
				pluginInstance->setPlayConfigDetails(
					2,
					2,
					sampleRate,
					blockSize);

				pluginInstance->prepareToPlay(sampleRate, blockSize);


				juce::ValueTree pluginDescription("pluginDescription");
				pluginDescription.setProperty("descriptionXML", pluginInstance->getPluginDescription().createXml()->createDocument(""), nullptr);
				pluginSettings.addChild(pluginDescription, -1, nullptr);

				juce::String base64Data = pluginStateRestorerValueTree.getChildWithName(pluginUuid.toString()).getChildWithName("pluginStateData").getProperty("stateData").toString();
				juce::MemoryBlock stateDataBlock;
				stateDataBlock.fromBase64Encoding(base64Data);
				pluginInstance->setStateInformation(
					stateDataBlock.getData(),
					static_cast<int>(stateDataBlock.getSize())
				);

				juce::ValueTree pluginStateData("pluginStateData");
				pluginStateData.setProperty("stateData", base64Data, nullptr);
				pluginSettings.addChild(pluginStateData, -1, nullptr);
			}
			else if (error.isNotEmpty())
			{
				DBG("Failed to load plugin: " + error);
			}
		};

	auto sampleRate = globalPlayhead.getSampleRate();
	auto blockSize = globalPlayhead.getBufferSize();

	pluginFormatManager.createPluginInstanceAsync(description,
		sampleRate,
		blockSize,
		onPluginLoaded);
}

void Plugin::doAddPlugin()
{
	//Add plugin
	loadPlugin(PluginActionStateManager::getInstance().getPluginDescription());

	pluginSettings.setProperty("Name", PluginActionStateManager::getInstance().getPluginDescription().name, nullptr);
	pluginSettings.setProperty("isOccupied", true, nullptr);

	PluginActionStateManager::getInstance().clear();

}

void Plugin::doOpenPlugin()
{

	if (pluginInstance == nullptr)
		return;

	if (pluginUI != nullptr && pluginUI->isVisible())
	{
		pluginUI->setVisible(false);
		pluginUI.reset();
	}


	if (pluginInstance->hasEditor())
	{
		doSavePlugin();

		// Create the plugin's editor
		std::unique_ptr<juce::AudioProcessorEditor> pluginEditor(pluginInstance->createEditorIfNeeded());

		if (pluginEditor != nullptr)
		{
			pluginUI.reset(new PluginUI(pluginEditor.release(), pluginUuid.toString()));
			startTimer(5000); //Se guardará el estado cada 5 segundos
		}
	}
	else
	{
		// For plugins without GUI or with custom handling
		juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon,
			"No GUI Available",
			"This plugin doesn't provide a graphical interface");
	}

	PluginActionStateManager::getInstance().clear();
}

void Plugin::doClosePlugin()
{
	if (pluginUI != nullptr && pluginUI->isVisible())
	{
		pluginUI->setVisible(false);
		pluginUI.reset();
	}
	stopTimer();
	doSavePlugin();
}

void Plugin::doRemovePlugin()
{
	if (pluginUI != nullptr && pluginUI->isVisible())
	{
		pluginUI->setVisible(false);
		pluginUI.reset();
	}
	if (pluginInstance)
	{
		pluginInstance->editorBeingDeleted(pluginInstance->getActiveEditor());
		pluginInstance->releaseResources();
	}
	pluginInstance.reset();

	pluginSettings.setProperty("Name", "Plugin", nullptr);
	pluginSettings.setProperty("isBypassed", false, nullptr);

	pluginSettings.setProperty("isOccupied", false, nullptr);
}

void Plugin::doSavePlugin()
{
	
	if (pluginSettings.getChildWithName("pluginDescription").isValid())
	{
		auto pluginDescription = pluginSettings.getChildWithName("pluginDescription");
		pluginDescription.setProperty("descriptionXML", pluginInstance->getPluginDescription().createXml()->createDocument(""), nullptr);
	}
	else
	{
		juce::ValueTree pluginDescription("pluginDescription");
		pluginDescription.setProperty("descriptionXML", pluginInstance->getPluginDescription().createXml()->createDocument(""), nullptr);
		pluginSettings.addChild(pluginDescription, -1, nullptr);
	}
	
	if (pluginSettings.getChildWithName("pluginStateData").isValid())
	{
		juce::MemoryBlock pluginStateDataBlock;
		pluginInstance->getStateInformation(pluginStateDataBlock);
		auto pluginStateData = pluginSettings.getChildWithName("pluginStateData");
		pluginStateData.setProperty("stateData", pluginStateDataBlock.toBase64Encoding(), nullptr);
	}
	else
	{
		juce::MemoryBlock pluginStateDataBlock;
		pluginInstance->getStateInformation(pluginStateDataBlock);
		juce::ValueTree pluginStateData("pluginStateData");
		pluginStateData.setProperty("stateData", pluginStateDataBlock.toBase64Encoding(), nullptr);
		pluginSettings.addChild(pluginStateData, -1, nullptr);
	}
}

void Plugin::timerCallback()
{
	doSavePlugin();
}
