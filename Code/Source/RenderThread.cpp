/*
  ==============================================================================

    RenderThread.cpp
    Created: 28 Feb 2025 10:53:34am
    Author:  jaezro

  ==============================================================================
*/

#include "RenderThread.h"

RenderThread::RenderThread(AudioSystemBus& src, const juce::File& f, double sampleRate, int bufferSize, double seconds_to_render)
    : Thread("Audio Render Thread")
    , audioSource(src)
    , outputFile(f)
    , sampleRate(sampleRate)
    , bufferSize(bufferSize)
    , seconds_to_render(seconds_to_render)
{

}

void RenderThread::run()
{
    // Do the actual rendering
    if (renderNow())
    {
        // Show success message on main thread
        messageToShow = "Rendering complete!\n" + outputFile.getFullPathName();
        triggerAsyncUpdate();
        SignalManagerUI::getInstance()->setSignal(SignalManagerUI::Signal::STOP_AUDIO);
    }
}

bool RenderThread::renderNow()
{
    DBG("RENDER STARTED!!!");

    std::unique_ptr<juce::FileOutputStream> outputStream(outputFile.createOutputStream());
    if (!outputStream || outputStream->failedToOpen())
        return false;

    // Create writer (same as before)
    juce::WavAudioFormat wavFormat;
    std::unique_ptr<juce::AudioFormatWriter> writer(
        wavFormat.createWriterFor(outputStream.get(), sampleRate, 2, 16, {}, 0));
    if (!writer)
        return false;

    outputStream.release();
    totalSamples = seconds_to_render * sampleRate; // Total samples to render
    audioSource.setTransportToBegin();

    DBG("Preparing thread");
    DBG("THREAD BUFFER SIZE , SAMPLE RATE: " << bufferSize << " , " << sampleRate);
    audioSource.prepareToPlay(bufferSize, sampleRate);
    juce::AudioBuffer<float> buffer(2, bufferSize);

    while (samplesRendered < totalSamples && !threadShouldExit())
    {
        DBG("RENDER PROCESS " < juce::String(samplesRendered));
        const int numToWrite = juce::jmin(bufferSize, static_cast<int>(totalSamples - samplesRendered));

        juce::AudioSourceChannelInfo info(buffer);
        info.numSamples = numToWrite;
        buffer.clear();
        audioSource.processNextAudioBlock(info);

        writer->writeFromAudioSampleBuffer(buffer, 0, numToWrite);
        samplesRendered += numToWrite;
    }

    audioSource.releaseResources();
    return !threadShouldExit(); // Return false if cancelled
}

void RenderThread::handleAsyncUpdate()
{
    if (messageToShow.isNotEmpty())
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon,
            "Rendering Complete",
            messageToShow);
    }
}
