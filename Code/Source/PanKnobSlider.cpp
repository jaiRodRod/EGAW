/*
  ==============================================================================

    PanKnobSlider.cpp
    Created: 5 Mar 2025 9:31:31am
    Author:  jaezro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PanKnobSlider.h"

//==============================================================================
PanKnobSlider::PanKnobSlider(juce::ValueTree& channelSettings) 
    : juce::Slider(SliderStyle::RotaryHorizontalVerticalDrag, TextEntryBoxPosition::TextBoxBelow)
    , channelSettings(channelSettings)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    SignalManagerUI::getInstance()->addListener(this);

    DBG(channelSettings.getType().toString());

    //setTextValueSuffix(" dB");
    setValue(channelSettings.getProperty("Pan"), juce::NotificationType::dontSendNotification);
    setSkewFactor(1, true); //What the hell??
    setRange(-1.0, 1.0, 0.01);
    setScrollWheelEnabled(true);
    setVelocityBasedMode(true);
    setVelocityModeParameters(2, 1);
    isTextActive = false;
    setDataMode(isTextActive);

}

PanKnobSlider::~PanKnobSlider()
{
    SignalManagerUI::getInstance()->removeListener(this);
}

void PanKnobSlider::valueChanged(juce::Value& value)
{
    auto signal = SignalManagerUI::getInstance()->getCurrentSignal();

    switch (signal)
    {
    case SignalManagerUI::Signal::RESTORE_UI_PARAMETERS:
        setValue(static_cast<float>(channelSettings.getProperty("Pan")));
        break;
    default:
        break;
    }
}

void PanKnobSlider::setDataMode(bool shouldBeOn)
{
    if (shouldBeOn)
    {
        //setTextValueSuffix(" pan");
        setTextBoxStyle(TextBoxBelow, false, getTextBoxWidth(), getTextBoxHeight());
    }
    else
    {
        setTextBoxStyle(NoTextBox, true, getTextBoxWidth(), getTextBoxHeight());
    }
    SignalManagerUI::getInstance()->setSignal(SignalManagerUI::Signal::RESIZED_TRIGGER);
}

void PanKnobSlider::mouseDrag(const juce::MouseEvent& event)
{
    channelSettings.setProperty("Pan", getValue(), nullptr);
    juce::Slider::mouseDrag(event);
}

void PanKnobSlider::mouseDoubleClick(const juce::MouseEvent& event)
{
    setValue(0.0);
    channelSettings.setProperty("Pan", getValue(), nullptr);
    juce::Slider::mouseDoubleClick(event);
}

void PanKnobSlider::mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel)
{
    channelSettings.setProperty("Pan", getValue(), nullptr);
    juce::Slider::mouseWheelMove(event, wheel);
}

void PanKnobSlider::mouseDown(const juce::MouseEvent& event)
{
    // Check if the right mouse button was clicked
    if (event.mods.isRightButtonDown())
    {
        // Call your custom function here
        isTextActive = !isTextActive;
        setDataMode(isTextActive);
    }
    else
    {
        // Call the base class method for default left-click behavior
        juce::Slider::mouseDown(event);
    }
}

/*
void PanKnobSlider::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("PanKnobSlider", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PanKnobSlider::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
*/