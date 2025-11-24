#pragma once

#include <JuceHeader.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "Waveform.h"

class WaveformArrayComponent : public juce::Component
{
public:
    WaveformArrayComponent(NonsenseAudioProcessor& p);
    ~WaveformArrayComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    int numWaves = 0;

    WaveformComponent waveform0;
    WaveformComponent waveform1;
    WaveformComponent waveform2;
    WaveformComponent waveform3;
    WaveformComponent waveform4;
    WaveformComponent waveform5;
    WaveformComponent waveform6;
    WaveformComponent waveform7;
    WaveformComponent waveform8;
    WaveformComponent waveform9;
};