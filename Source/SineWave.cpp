
#include "SineWave.h"

void SineWave::prepare(double SampleRate, int channelCount) 
{
	currentSampleRate = (float)SampleRate;
	timeIncrement = 1.0f / currentSampleRate;
	currentPhasePerChannel.resize(channelCount, 0.0f);
	previousOutputValuePerChannel.resize(channelCount, 0.0f);

	smoothedAmplitude.reset(currentSampleRate, 0.15f);
	smoothedFrequency.reset(currentSampleRate, 0.1f);

	smoothedFrequency.setCurrentAndTargetValue(frequency);
	smoothedAmplitude.setCurrentAndTargetValue(amplitude);
	smoothedPulse.setCurrentAndTargetValue(pulseLevel);
	recalculatePhaseError();
	smoothedPhaseError.setCurrentAndTargetValue(phaseError);

	randomDetune = float(rand() % 2000) / 1000.0 - 1.0;
}

float SineWave::sampleWave(float samplePoint, waveType type)
{
	const float pi = juce::MathConstants<float>::pi;
	if(type == waveType::SINE)
		return std::sinf(samplePoint);
	if (type == waveType::SQUARE)
		return samplePoint > pi ? 1.0 : -1.0;
	if(type == waveType::TRIANGLE)
		return samplePoint > pi ? 3.0 - samplePoint / pi * 2.0 : samplePoint / pi * 2.0 - 1.0;
	if (type == waveType::SAW)
		return samplePoint / pi - 1.0;
	return float(rand() % 2000) / 1000.0 - 1.0;
}

void SineWave::process(juce::AudioBuffer<float>& buffer)
{

	if (currentPhasePerChannel.size() != buffer.getNumChannels())
		return;

	if (!enabled || !adsr.isActive())
		return;

	const float alpha = (1.0 / currentSampleRate) / (1.0 / lowPassFrequency + 1.0 / currentSampleRate);

	for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
	{
		auto* output = buffer.getWritePointer(channel);
		for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
		{
			
			const float phaseIncrement = (1.0 + 0.1 * outOfTune * randomDetune) * (juce::MathConstants<float>::twoPi * smoothedFrequency.getNextValue() / currentSampleRate * (1 + smoothedPulse.getNextValue() * sampleWave(currentPhasePerChannel[channel], wave)) - pulseLevel * pulseLevel * waveTypeScaling[wave]) / smoothedPhaseError.getNextValue();
			float prefilterOutput = adsr.getNextSample() * smoothedAmplitude.getNextValue() * sampleWave(currentPhasePerChannel[channel], wave);

			previousOutputValuePerChannel[channel] = previousOutputValuePerChannel[channel] + (prefilterOutput - previousOutputValuePerChannel[channel]) * alpha;

			output[sample] += (previousOutputValuePerChannel[channel] * lowPassWet + prefilterOutput * (1.0 - lowPassWet)) * amplitude * velocity;

			currentPhasePerChannel[channel] += phaseIncrement;
			if (currentPhasePerChannel[channel] > juce::MathConstants<float>::twoPi)
			{
				currentPhasePerChannel[channel] -= juce::MathConstants<float>::twoPi;
			}

		}
	}
}

void SineWave::recalculatePhaseError() {
	float phaseError1 = 1.0;
	for (int j = 0; j < 6; j++)
	{
		float newPhaseError = 0.0f;
		for (int i = 0; i < 1000; i++)
		{
			const float phaseIncrement = (juce::MathConstants<float>::twoPi * 0.001f * (1.0f + pulseLevel * sampleWave(newPhaseError, wave)) - pulseLevel * pulseLevel * waveTypeScaling[wave]) / phaseError1;
			newPhaseError += phaseIncrement;
		}

		newPhaseError /= juce::MathConstants<float>::twoPi;

		phaseError1 *= newPhaseError;
		if (abs(newPhaseError - 1.0f) < 0.01f)
		{
			break;
		}
	}
	float phaseError2 = 1.0;
	for (int j = 0; j < 5; j++)
	{
		float newPhaseError = 0.0f;
		for (int i = 0; i < 1000; i++)
		{
			const float phaseIncrement = (juce::MathConstants<float>::twoPi * 0.001f * (1.0f + pulseLevel * sampleWave(newPhaseError, wave)) - pulseLevel * pulseLevel * waveTypeScaling[wave]) / phaseError2;
			newPhaseError += phaseIncrement;
		}

		newPhaseError /= juce::MathConstants<float>::twoPi;

		phaseError2 *= newPhaseError;
		if (abs(newPhaseError - 1.0f) < 0.01f)
		{
			break;
		}
	}
	float phaseError3 = 1.0;
	for (int j = 0; j < 7; j++)
	{
		float newPhaseError = 0.0f;
		for (int i = 0; i < 1000; i++)
		{
			const float phaseIncrement = (juce::MathConstants<float>::twoPi * 0.001f * (1.0f + pulseLevel * sampleWave(newPhaseError, wave)) - pulseLevel * pulseLevel * waveTypeScaling[wave]) / phaseError3;
			newPhaseError += phaseIncrement;
		}

		newPhaseError /= juce::MathConstants<float>::twoPi;

		phaseError3 *= newPhaseError;
		if (abs(newPhaseError - 1.0f) < 0.01f)
		{
			break;
		}
	}
	if (abs(phaseError3 - phaseError1) < 0.01) {
		phaseError = sqrt(phaseError1 * phaseError2);
	}
	else {
		phaseError = powf(phaseError1 * phaseError2 * phaseError3, 0.33333);
	}
	if (phaseError < 0.01) {
		phaseError = 0.01;
	}
	smoothedPhaseError.setTargetValue(phaseError);
}