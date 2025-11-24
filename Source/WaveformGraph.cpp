
#include "WaveformGraph.h"
#include "LookAndFeel.h"

WaveformGraphComponent::WaveformGraphComponent() {

}
WaveformGraphComponent::~WaveformGraphComponent() {

}

void WaveformGraphComponent::paint(juce::Graphics& g) {
	juce::Rectangle bounds = getBounds();

	g.fillAll(NonsenseLookAndFeel::backgroundColor1);
	g.setColour(NonsenseLookAndFeel::penColor);

	g.drawRect(0, 0, bounds.getWidth(), bounds.getHeight());

	float currentPhase = 0.0;
	float previousHeight = 0.0;
	for (int sample = 0; sample < 100; ++sample)
	{

		const float phaseIncrement = (juce::MathConstants<float>::twoPi * wave.get_frequency() / 50.0 * 
			(1 + wave.get_pulse() * wave.sampleWave(currentPhase, wave.get_wave_type())) - 
			wave.get_pulse() * wave.get_pulse() * wave.get_wave_type_scaling())
			/ wave.get_phase_error();
		float height = wave.sampleWave(currentPhase, wave.get_wave_type()) * wave.get_amplitude();


		currentPhase += phaseIncrement;
		if (currentPhase > juce::MathConstants<float>::twoPi)
		{
			currentPhase -= juce::MathConstants<float>::twoPi;
		}
		if (sample > 0) {
			g.drawLine((float(sample - 1)) / 100.0 * bounds.getWidth(),
				(previousHeight + 1.0) * (bounds.getHeight() - 2.0) / 2.0 + 1.0,
				float(sample) / 100.0 * bounds.getWidth(),
				(height + 1.0) * (bounds.getHeight() - 2.0) / 2.0 + 1.0);
		}
		previousHeight = height;
	}
}
void WaveformGraphComponent::resized() {

}