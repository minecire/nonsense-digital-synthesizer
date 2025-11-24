
#include "WaveformArray.h"

WaveformArrayComponent::WaveformArrayComponent(NonsenseAudioProcessor& p)
    :
    waveform0(p, 0),
    waveform1(p, 1),
    waveform2(p, 2),
    waveform3(p, 3),
    waveform4(p, 4),
    waveform5(p, 5),
    waveform6(p, 6),
    waveform7(p, 7),
    waveform8(p, 8),
    waveform9(p, 9)
{

    addAndMakeVisible(waveform0);
    addAndMakeVisible(waveform1);
    addAndMakeVisible(waveform2);
    addAndMakeVisible(waveform3);
    addAndMakeVisible(waveform4);
    addAndMakeVisible(waveform5);
    addAndMakeVisible(waveform6);
    addAndMakeVisible(waveform7);
    addAndMakeVisible(waveform8);
    addAndMakeVisible(waveform9);
}
WaveformArrayComponent::~WaveformArrayComponent()
{

}
void WaveformArrayComponent::paint(juce::Graphics& g)
{

}
void WaveformArrayComponent::resized()
{
    waveform0.setBounds(10, 10 + 110 * 0, 550, 100);
    waveform1.setBounds(10, 10 + 110 * 1, 550, 100);
    waveform2.setBounds(10, 10 + 110 * 2, 550, 100);
    waveform3.setBounds(10, 10 + 110 * 3, 550, 100);
    waveform4.setBounds(10, 10 + 110 * 4, 550, 100);
    waveform5.setBounds(10, 10 + 110 * 5, 550, 100);
    waveform6.setBounds(10, 10 + 110 * 6, 550, 100);
    waveform7.setBounds(10, 10 + 110 * 7, 550, 100);
    waveform8.setBounds(10, 10 + 110 * 8, 550, 100);
    waveform9.setBounds(10, 10 + 110 * 9, 550, 100);
}