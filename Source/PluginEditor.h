/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "WaveformArray.h"
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "LookAndFeel.h"

//==============================================================================
/**
*/
class NonsenseAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    NonsenseAudioProcessorEditor(NonsenseAudioProcessor&);
    ~NonsenseAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NonsenseAudioProcessor& audioProcessor;

    WaveformArrayComponent waveforms;

    juce::Viewport waveformsViewport;

    NonsenseLookAndFeel CustomLNF;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NonsenseAudioProcessorEditor)
};
