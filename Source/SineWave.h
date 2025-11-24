#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

enum class waveType {
	SINE = 1,
	SQUARE = 2,
	TRIANGLE = 3,
	SAW = 4,
	NOISE = 5
};

class SineWave
{
public:
	void prepare(double sampleRate, int channelCount);
	void process(juce::AudioBuffer<float>& buffer);

	float get_amplitude() const { return amplitude; }
	void set_amplitude(float newAmplitude) { amplitude = newAmplitude; smoothedAmplitude.setTargetValue(newAmplitude); }

	float get_frequency() const { return frequency; }
	void set_frequency(float newFrequency) { frequency = newFrequency; smoothedFrequency.setTargetValue(newFrequency);}

	float get_pulse() const { return pulseLevel; }
	void set_pulse(float newPulse) { float oldPulse = pulseLevel; pulseLevel = newPulse; smoothedPulse.setTargetValue(newPulse); if (oldPulse != newPulse) { recalculatePhaseError(); } }

	waveType get_wave_type() const { return wave; }
	void set_wave_type(waveType waveType) {
		int oldWave = (int)wave;
		wave = waveType; 
		if (oldWave != (int)waveType) {
			recalculatePhaseError();
		}
	}

	void set_adsr(float atk, float dcy, float sus, float rls) {
		adsr.setParameters(juce::ADSR::Parameters::Parameters(atk, dcy, sus, rls));
	}

	void start_adsr() {
		adsr.noteOn();
	}
	
	void end_adsr() {
		adsr.noteOff();
	}

	void set_low_pass(float freq, float wet) {
		lowPassFrequency = freq;
		lowPassWet = wet;
	}

	bool get_enabled() const { return enabled; }
	void set_enabled(bool newEnabled) { enabled = newEnabled; }

	void set_out_of_tune(float newOutOfTune) { outOfTune = newOutOfTune; }

	void set_velocity(float newVelocity) { velocity = newVelocity; }

	float sampleWave(float samplePoint, waveType type);

	float get_wave_type_scaling() { return waveTypeScaling[wave]; }

	float get_phase_error() { return phaseError; }

private:

	float phaseError = 1.0f;
	float amplitude = 0.0f;
	float frequency = 440.0f;
	float currentSampleRate = 0.0f;
	float timeIncrement = 0.0f;
	float pulseLevel = 0.5f;

	float timeIntoNote = 0.0f;

	float lowPassFrequency = 1760.0f;

	float lowPassWet = 0.5f;

	float outOfTune = 0.0f;
	float randomDetune = 0.0f;

	float velocity = 1.0;

	enum waveType wave = waveType::SINE;
	bool enabled = false;

	std::map<waveType, float> waveTypeScaling = {
		{waveType::SINE, -0.000054},
		{waveType::SQUARE, -0.000077},
		{waveType::TRIANGLE, 0.000187},
		{waveType::SAW, -0.000036},
		{waveType::NOISE, 0.0},
	};

	std::vector<float>previousOutputValuePerChannel;
	std::vector<float> currentPhasePerChannel;

	juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedAmplitude;
	juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative> smoothedFrequency;
	juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative> smoothedPulse;
	juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative> smoothedPhaseError;

	juce::ADSR adsr;

	void recalculatePhaseError();
};