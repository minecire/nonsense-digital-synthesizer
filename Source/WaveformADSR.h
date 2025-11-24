
#include <JuceHeader.h>

class WaveformADSRComponent : public juce::Component
{
public:

	WaveformADSRComponent();
	~WaveformADSRComponent() override;

	void paint(juce::Graphics&) override;
	void resized() override;

private:
	float attackTime = 1.0f;
	float decayTime = 1.0f;
	float sustainLevel = 0.5f;
	float releaseTime = 2.0f;
};