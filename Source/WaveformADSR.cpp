
#include "WaveformADSR.h"
#include "LookAndFeel.h"

WaveformADSRComponent::WaveformADSRComponent() {

}
WaveformADSRComponent::~WaveformADSRComponent() {

}

void WaveformADSRComponent::paint(juce::Graphics& g) {
	juce::Rectangle bounds = getBounds();

	g.fillAll(NonsenseLookAndFeel::backgroundColor1);
	g.setColour(NonsenseLookAndFeel::penColor);

	g.drawRect(0, 0, bounds.getWidth(), bounds.getHeight());

	g.drawLine(1.0 / 10.0 * bounds.getWidth(), bounds.getHeight(), (1.0 + attackTime) / 10.0 * bounds.getWidth(), bounds.getHeight() * 0.9);
	g.drawLine((1.0 + attackTime) / 10.0 * bounds.getWidth(), 0, (1.0 + attackTime + decayTime) / 10.0 * bounds.getWidth(), bounds.getHeight() * (1.0 - sustainLevel));
	g.drawLine((1.0 + attackTime + decayTime) / 10.0 * bounds.getWidth(), bounds.getHeight() * (1.0 - sustainLevel), (1.0 + attackTime + decayTime + 4.0) / 10.0 * bounds.getWidth(), bounds.getHeight() * (1.0 - sustainLevel));
	g.drawLine((1.0 + attackTime + decayTime + 4.0) / 10.0 * bounds.getWidth(), bounds.getHeight() * (1.0 - sustainLevel), (1.0 + attackTime + decayTime + 4.0 + releaseTime) / 10.0 * bounds.getWidth(), bounds.getHeight());
	
}
void WaveformADSRComponent::resized() {

}