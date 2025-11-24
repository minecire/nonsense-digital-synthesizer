#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <JuceHeader.h>
#include "SineWave.h"

class WaveformGraphComponent : public juce::Component
{
public:

	WaveformGraphComponent();
	~WaveformGraphComponent() override;

	void paint(juce::Graphics&) override;
	void resized() override;

	void setParameters(waveType type, float pulseWidth, float pitchRough, float pitchFine, float amplitude) {
		wave.set_pulse(pulseWidth);
		wave.set_frequency(pow(2, (pitchRough + pitchFine) / 12.0));
		wave.set_wave_type(type);
		wave.set_amplitude(amplitude);
	}

private:
	SineWave wave;
};