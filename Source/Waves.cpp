
#include "Waves.h"

void Waves::prepare(double sampleRate, int channelCount)
{
	for (int i = 0; i < 128; i++)
	{
		Waves::sineWaves[i].prepare(sampleRate, channelCount);
		Waves::sineWaves[i].set_frequency(17.323 * powf(2.0f, i / 12.0f) * pitchShift);
		Waves::sineWaves[i].set_amplitude(amplitude);
	}
}

void Waves::process(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	for (const juce::MidiMessageMetadata metadata : midiMessages)
	{
		juce::MidiMessage message = metadata.getMessage();
		if (!message.isNoteOnOrOff())
			continue;
		if (message.isNoteOn()) {
			Waves::sineWaves[message.getNoteNumber()].set_enabled(true);
			Waves::sineWaves[message.getNoteNumber()].start_adsr();
			Waves::sineWaves[message.getNoteNumber()].set_velocity(message.getFloatVelocity());
		}
		else
			Waves::sineWaves[message.getNoteNumber()].end_adsr();
	}

	for (int i = 0; i < 128; i++)
	{
		Waves::sineWaves[i].set_frequency(22.5f * powf(2.0f, i / 12.0f) * pitchShift);
	}
	for (int i = 0; i < 128; i++)
	{
		Waves::sineWaves[i].process(buffer);
	}
}