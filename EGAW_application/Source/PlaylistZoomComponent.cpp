/*
  ==============================================================================

    PlaylistZoomComponent.cpp
    Created: 10 Apr 2025 7:27:50pm
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistZoomComponent.h"

//==============================================================================
PlaylistZoomComponent::PlaylistZoomComponent(juce::ValueTree& projectData)
	: flexBox(juce::FlexBox::Direction::column, juce::FlexBox::Wrap::noWrap
        , juce::FlexBox::AlignContent::center, juce::FlexBox::AlignItems::center
        , juce::FlexBox::JustifyContent::center)
    , projectData(projectData)
	, zoomInButton("+")
	, zoomLabel()
	, zoomOutButton("-")
{
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.
	projectData.addListener(this);

	zoomInButton.setLookAndFeel(&zoomInButton_LF);
    zoomInButton.onClick = [this] { zoomIn(); };
	addAndMakeVisible(zoomInButton);

	zoomLabel.setText("Zoom: " + textZoomLabel((double)projectData.getProperty("Zoom")), juce::dontSendNotification);
	zoomLabel.setJustificationType(juce::Justification::centred);
	zoomLabel.setColour(juce::Label::backgroundColourId, colourConstants::dark_slate_blue_pressed);
	zoomLabel.setColour(juce::Label::textColourId, juce::Colours::white);
	addAndMakeVisible(zoomLabel);

	zoomOutButton.setLookAndFeel(&zoomOutButton_LF);
	zoomOutButton.onClick = [this] { zoomOut(); };
	addAndMakeVisible(zoomOutButton);
}

PlaylistZoomComponent::~PlaylistZoomComponent()
{
    projectData.removeListener(this);
}

void PlaylistZoomComponent::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
	if (projectData == treeWhosePropertyHasChanged)
	{
		if (property == juce::Identifier("Zoom"))
		{
			zoomLabel.setText("Zoom: " + textZoomLabel((double)projectData.getProperty("Zoom")), juce::dontSendNotification);
			resized();
			repaint();
			juce::MessageManager::callAsync([this] {
				SignalManagerUI::getInstance().setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
			});
		}
	}
}



void PlaylistZoomComponent::paint (juce::Graphics& g)
{
    g.fillAll (colourConstants::dark_slate_blue);   // clear the background
}

void PlaylistZoomComponent::resized()
{
	auto footerBounds = getLocalBounds();

	flexBox.items.clear();

	auto zoomInHeight = (float)(footerBounds.getHeight() / 3);

	flexBox.items.add(juce::FlexItem(zoomInButton).withMinHeight(zoomInHeight).withMinWidth(footerBounds.getWidth()));
	flexBox.items.add(juce::FlexItem(zoomLabel).withMinHeight(zoomInHeight).withMinWidth(footerBounds.getWidth() - 2.0f));
	flexBox.items.add(juce::FlexItem(zoomOutButton).withMinHeight(zoomInHeight).withMinWidth(footerBounds.getWidth()));

	flexBox.performLayout(footerBounds);

	auto labelFont = juce::Font(juce::Font::getDefaultSansSerifFontName(),
		zoomInHeight * 0.8f,   // 60% of button height
		juce::Font::plain);

	zoomLabel.setFont(labelFont);
	if (footerBounds.getWidth() - 2.0f < 80)
	{
		zoomLabel.setText(textZoomLabel((double)projectData.getProperty("Zoom")), juce::dontSendNotification);
	}
	else
	{
		zoomLabel.setText("Zoom: " + textZoomLabel((double)projectData.getProperty("Zoom")), juce::dontSendNotification);
	}
}

void PlaylistZoomComponent::zoomIn()
{
	auto value = (double)projectData.getProperty("Zoom");
	if (value == playlistThumbnailZoomConstants::x0_5)
	{
		projectData.setProperty("Zoom", playlistThumbnailZoomConstants::x1, nullptr);
	}
	else if (value == playlistThumbnailZoomConstants::x1)
	{
		projectData.setProperty("Zoom", playlistThumbnailZoomConstants::x2, nullptr);
	}
	else if (value == playlistThumbnailZoomConstants::x2)
	{
		projectData.setProperty("Zoom", playlistThumbnailZoomConstants::x5, nullptr);
	}
	else if (value == playlistThumbnailZoomConstants::x5)
	{
		projectData.setProperty("Zoom", playlistThumbnailZoomConstants::x10, nullptr);
	}
	else if (value == playlistThumbnailZoomConstants::x10)
	{
		// Do nothing, already at max zoom
	}
}

juce::String PlaylistZoomComponent::textZoomLabel(double)
{
	juce::String textZoom("x1");
	if ((double)projectData.getProperty("Zoom") == playlistThumbnailZoomConstants::x0_5)
	{
		textZoom = "x0.5";
	}
	else if ((double)projectData.getProperty("Zoom") == playlistThumbnailZoomConstants::x1)
	{
		textZoom = "x1";
	}
	else if ((double)projectData.getProperty("Zoom") == playlistThumbnailZoomConstants::x2)
	{
		textZoom = "x2";
	}
	else if ((double)projectData.getProperty("Zoom") == playlistThumbnailZoomConstants::x5)
	{
		textZoom = "x5";
	}
	else if ((double)projectData.getProperty("Zoom") == playlistThumbnailZoomConstants::x10)
	{
		textZoom = "x10";
	}
	return textZoom;
}

void PlaylistZoomComponent::zoomOut()
{
	auto value = (double)projectData.getProperty("Zoom");
	if (value == playlistThumbnailZoomConstants::x10)
	{
		projectData.setProperty("Zoom", playlistThumbnailZoomConstants::x5, nullptr);
	}
	else if (value == playlistThumbnailZoomConstants::x5)
	{
		projectData.setProperty("Zoom", playlistThumbnailZoomConstants::x2, nullptr);
	}
	else if (value == playlistThumbnailZoomConstants::x2)
	{
		projectData.setProperty("Zoom", playlistThumbnailZoomConstants::x1, nullptr);
	}
	else if (value == playlistThumbnailZoomConstants::x1)
	{
		projectData.setProperty("Zoom", playlistThumbnailZoomConstants::x0_5, nullptr);
	}
	else if (value == playlistThumbnailZoomConstants::x0_5)
	{
		// Do nothing, already at min zoom
	}
}
