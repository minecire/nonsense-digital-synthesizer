/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "Waves.h"

//==============================================================================
/**
*/
class NonsenseAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    NonsenseAudioProcessor();
    ~NonsenseAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    int getNumWaves() const { return numWaves; }

    juce::AudioProcessorValueTreeState& getState() { return state; }

    std::vector<int> waveformBoxPositions;
    std::vector<float> playingValues;

private:
    int numWaves = 10;
    std::vector<Waves> waves;

    juce::AudioProcessorValueTreeState state;

    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

    std::vector<std::atomic<float>*> playParams;
    std::vector<std::atomic<float>*> pulseParams;
    std::vector<std::atomic<float>*> waveParams;


    std::vector<std::atomic<float>*> attackParams;
    std::vector<std::atomic<float>*> decayParams;
    std::vector<std::atomic<float>*> sustainParams;
    std::vector<std::atomic<float>*> releaseParams;

    std::vector<std::atomic<float>*> volumeParams;

    std::vector<std::atomic<float>*> roughPitchParams;
    std::vector<std::atomic<float>*> finePitchParams;

    std::vector<std::atomic<float>*> lowPassFreqParams;
    std::vector<std::atomic<float>*> lowPassWetParams;

    std::vector<std::atomic<float>*> outOfTuneParams;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NonsenseAudioProcessor)
};
