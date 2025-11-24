#pragma once

#include "SineWave.h"

class Waves {
public:
	void prepare(double sampleRate, int channelCount);
	void process(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);


	float get_amplitude() const { return amplitude; }
	void set_amplitude(float newAmplitude) { 
		amplitude = newAmplitude;
		for (int i = 0; i < 128; i++) {
			sineWaves[i].set_amplitude(newAmplitude);
		}
	}


	float get_pulse() const { return pulseLevel; }
	void set_pulse(float newPulse) {
		pulseLevel = newPulse;
		for (int i = 0; i < 128; i++) {
			sineWaves[i].set_pulse(newPulse);
		}
	}

	void set_adsr(float atk, float dcy, float sus, float rls) {
		attack = exp(atk);
		decay = exp(dcy);
		sustain = sus;
		release = exp(rls);
		for (int i = 0; i < 128; i++) {
			sineWaves[i].set_adsr(attack, decay, sustain, release);
		}
	}

	waveType get_wave_type() const { return wave; }
	void set_wave_type(waveType waveType) { 
		wave = waveType;
		for (int i = 0; i < 128; i++) {
			sineWaves[i].set_wave_type(waveType);
		}
	}

	void set_pitch_shift(float newPitchShift) {
		pitchShift = powf(2.0f, newPitchShift / 12.0f);
		for (int i = 0; i < 128; i++) {
			sineWaves[i].set_frequency(22.5f * powf(2.0f, i / 12.0f) * pitchShift);
		}
	}

	void set_low_pass(float newFreq, float newWet) {
		lowPassFrequency = powf(2.0, newFreq * 2.0 - 12.0);
		lowPassWet = newWet;
		for (int i = 0; i < 128; i++) {
			sineWaves[i].set_low_pass(lowPassFrequency, lowPassWet);
		}

	}

	void set_out_of_tune(float newOutOfTune) {
		outOfTune = newOutOfTune;
		for (int i = 0; i < 128; i++) {
			sineWaves[i].set_out_of_tune(outOfTune);
		}
	}

private:
	float amplitude = 0.1f;
	float pulseLevel = 0.5f;
	float currentSampleRate = 0.0f;
	float pitchShift = 1.0f;

	float attack = 0.01f;
	float decay = 0.01f;
	float sustain = 1.0f;
	float release = 0.01f;

	float lowPassFrequency = 440.0f;
	float lowPassWet = 0.5f;

	float outOfTune = 0.0f;

	waveType wave = waveType::SINE;

	SineWave sineWaves[128];
};