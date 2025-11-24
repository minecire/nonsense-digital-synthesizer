#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "WaveformGraph.h"
#include "WaveformADSR.h"

class WaveformComponent : public juce::Component
{
public:

	WaveformComponent(NonsenseAudioProcessor& p, int index);
	~WaveformComponent() override;

	void paint(juce::Graphics&) override;
	void resized() override;

private:

	int index = 0;

	WaveformGraphComponent graph;
	WaveformADSRComponent adsr;

	juce::Slider pulseSlider;
	juce::ComboBox waveFormBox;

	juce::Slider pitchAdjustFine;
	juce::Slider pitchAdjustRough;

	juce::Slider attackSlider;
	juce::Slider decaySlider;
	juce::Slider sustainSlider;
	juce::Slider releaseSlider;

	juce::TextButton playButton;
	juce::Slider volumeSlider;

	juce::Slider lowPassFreqSlider;
	juce::Slider lowPassWetSlider;

	juce::Slider outOfTuneSlider;

	juce::AudioProcessorValueTreeState::SliderAttachment pulseSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveFormBoxAttachment;

	juce::AudioProcessorValueTreeState::SliderAttachment attackAttachment;
	juce::AudioProcessorValueTreeState::SliderAttachment decayAttachment;
	juce::AudioProcessorValueTreeState::SliderAttachment sustainAttachment;
	juce::AudioProcessorValueTreeState::SliderAttachment releaseAttachment;

	juce::AudioProcessorValueTreeState::ButtonAttachment playButtonAttachment;
	juce::AudioProcessorValueTreeState::SliderAttachment volumeAttachment;

	juce::AudioProcessorValueTreeState::SliderAttachment pitchFineAttachment;
	juce::AudioProcessorValueTreeState::SliderAttachment pitchRoughAttachment;


	juce::AudioProcessorValueTreeState::SliderAttachment lowPassFreqAttachment;
	juce::AudioProcessorValueTreeState::SliderAttachment lowPassWetAttachment;

	juce::AudioProcessorValueTreeState::SliderAttachment outOfTuneAttachment;
};