/*
  ==============================================================================

    AudioTimeAdjusterWindow.h
    Created: 6 Apr 2025 8:14:05pm
    Author:  admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class AudioTimeAdjusterWindow : public juce::Component,
                               public juce::Button::Listener
{
public:
    using Callback = std::function<void(double newStart, double newEnd, bool accepted)>;

    AudioTimeAdjusterWindow(double audioFileStart, 
                           double audioFileEnd, 
                           double audioFileLength,
                           Callback callback)
        : startTimeEditor("Start Time"),
          endTimeEditor("End Time"),
          callbackFunction(callback),
          fileLength(audioFileLength)
    {
        // Set up the message label
        messageLabel.setText("Adjust the time start and end of your audio channel in seconds", 
                           juce::NotificationType::dontSendNotification);
        messageLabel.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(messageLabel);

        // Set up the start time editor
        startTimeEditor.setText(juce::String(audioFileStart), juce::NotificationType::dontSendNotification);
        startTimeEditor.setInputRestrictions(20, "0123456789.");
        addAndMakeVisible(startTimeEditor);

        // Set up the end time editor
        endTimeEditor.setText(juce::String(audioFileEnd), juce::NotificationType::dontSendNotification);
        endTimeEditor.setInputRestrictions(20, "0123456789.");
        addAndMakeVisible(endTimeEditor);

        // Set up the length info label
        updateLengthInfo();
        addAndMakeVisible(lengthInfoLabel);

        // Set up the buttons
        acceptButton.setButtonText("Accept");
        acceptButton.addListener(this);
        addAndMakeVisible(acceptButton);

        cancelButton.setButtonText("Cancel");
        cancelButton.addListener(this);
        addAndMakeVisible(cancelButton);

        // Set up error label (initially hidden)
        errorLabel.setColour(juce::Label::textColourId, juce::Colours::red);
        errorLabel.setJustificationType(juce::Justification::centred);
        addChildComponent(errorLabel);

        // Set window size
        setSize(400, 220);
    }

    void resized() override
    {
        auto bounds = getLocalBounds().reduced(10);
        
        // First row - message
        auto messageRow = bounds.removeFromTop(30);
        messageLabel.setBounds(messageRow);
        
        // Second row - editors and length info
        bounds.removeFromTop(10); // spacing
        auto editorRow = bounds.removeFromTop(30);
        
        auto startEditorArea = editorRow.removeFromLeft(editorRow.getWidth() / 2 - 5);
        startTimeEditor.setBounds(startEditorArea.removeFromTop(24));
        
        editorRow.removeFromLeft(10); // spacing between editors
        
        auto endEditorArea = editorRow;
        endTimeEditor.setBounds(endEditorArea.removeFromTop(24));
        
        // Length info below editors
        bounds.removeFromTop(5);
        lengthInfoLabel.setBounds(bounds.removeFromTop(20));
        
        // Error message (will be shown when needed)
        bounds.removeFromTop(5);
        errorLabel.setBounds(bounds.removeFromTop(20));
        
        // Buttons at bottom
        bounds.removeFromTop(10);
        auto buttonRow = bounds.removeFromTop(30);
        
        auto buttonWidth = buttonRow.getWidth() / 2 - 10;
        acceptButton.setBounds(buttonRow.removeFromLeft(buttonWidth));
        buttonRow.removeFromLeft(20); // spacing
        cancelButton.setBounds(buttonRow);
    }

    void buttonClicked(juce::Button* button) override
    {
        if (button == &acceptButton)
        {
            double newStart = startTimeEditor.getText().getDoubleValue();
            double newEnd = endTimeEditor.getText().getDoubleValue();
            
            // Validate input
            if (newStart >= newEnd)
            {
                errorLabel.setText("End time must be after start time!", juce::dontSendNotification);
                errorLabel.setVisible(true);
                resized(); // Update layout
                return;
            }
            
            if (newEnd > fileLength)
            {
                errorLabel.setText("End time exceeds file length!", juce::dontSendNotification);
                errorLabel.setVisible(true);
                resized(); // Update layout
                return;
            }
            
            if (callbackFunction)
                callbackFunction(newStart, newEnd, true);
                
            closeDialog();
        }
        else if (button == &cancelButton)
        {
            if (callbackFunction)
                callbackFunction(0, 0, false);
                
            closeDialog();
        }
    }

private:
    juce::Label messageLabel;
    juce::Label lengthInfoLabel;
    juce::Label errorLabel;
    juce::TextEditor startTimeEditor;
    juce::TextEditor endTimeEditor;
    juce::TextButton acceptButton;
    juce::TextButton cancelButton;
    
    Callback callbackFunction;
    double fileLength;

    void updateLengthInfo()
    {
        lengthInfoLabel.setText("(The file is " + juce::String(fileLength) + " seconds long)", 
                               juce::NotificationType::dontSendNotification);
    }

    void closeDialog()
    {
        if (auto* dw = findParentComponentOfClass<juce::DialogWindow>())
            dw->exitModalState(0);
    }
};

// Helper function to show the popup (unchanged)
void showAudioTimeAdjusterPopup(double audioFileStart,
    double audioFileEnd,
    double audioFileLength,
    std::function<void(double newStart, double newEnd, bool accepted)> callback);
