
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Waveform.h"
#include "LookAndFeel.h"

WaveformComponent::WaveformComponent(NonsenseAudioProcessor& p, int index) : 
    pulseSliderAttachment(p.getState(), "Pulse" + std::to_string(index), pulseSlider),
    playButtonAttachment(p.getState(), "Play" + std::to_string(index), playButton),
    attackAttachment(p.getState(), "Attack" + std::to_string(index), attackSlider),
    decayAttachment(p.getState(), "Decay" + std::to_string(index), decaySlider),
    sustainAttachment(p.getState(), "Sustain" + std::to_string(index), sustainSlider),
    releaseAttachment(p.getState(), "Release" + std::to_string(index), releaseSlider),
    volumeAttachment(p.getState(), "Volume" + std::to_string(index), volumeSlider),
    pitchRoughAttachment(p.getState(), "PitchR" + std::to_string(index), pitchAdjustRough),
    pitchFineAttachment(p.getState(), "PitchF" + std::to_string(index), pitchAdjustFine),
    lowPassWetAttachment(p.getState(), "LowPW" + std::to_string(index), lowPassWetSlider),
    lowPassFreqAttachment(p.getState(), "LowPF" + std::to_string(index), lowPassFreqSlider),
    outOfTuneAttachment(p.getState(), "OutOfTune" + std::to_string(index), outOfTuneSlider)

{
    pulseSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    pulseSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 25);
    pulseSlider.setRange(0.0f, 0.99f, 0.01f);
    pulseSlider.setColour(juce::Slider::backgroundColourId, NonsenseLookAndFeel::fancyPenColors[index]);
    pulseSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0));
    pulseSlider.setColour(juce::Slider::textBoxTextColourId, NonsenseLookAndFeel::fancyPenColors[index]);


    waveFormBox.addItem("Sine", 1);
    waveFormBox.addItem("Square", 2);
    waveFormBox.addItem("Triangle", 3);
    waveFormBox.addItem("Saw", 4);
    waveFormBox.addItem("Noise", 5);
    waveFormBox.setColour(juce::ComboBox::backgroundColourId, NonsenseLookAndFeel::fancyPenColors[index]);
    waveFormBox.setColour(juce::ComboBox::textColourId, NonsenseLookAndFeel::fancyPenColors[index]);

    waveFormBoxAttachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(p.getState(), "WaveType" + std::to_string(index), waveFormBox));

    
    if(p.waveformBoxPositions.size() > index)
        waveFormBox.setSelectedId(p.waveformBoxPositions[index]);

    attackSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    attackSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    attackSlider.setRange(-10.0f, 3.0f, 0.01f);
    attackSlider.setColour(juce::Slider::backgroundColourId, NonsenseLookAndFeel::fancyPenColors[index]);

    decaySlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    decaySlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    decaySlider.setRange(-10.0f, 3.0f, 0.01f);
    decaySlider.setColour(juce::Slider::backgroundColourId, NonsenseLookAndFeel::fancyPenColors[index]);

    sustainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    sustainSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    sustainSlider.setRange(0.0f, 1.0f, 0.01f);
    sustainSlider.setColour(juce::Slider::backgroundColourId, NonsenseLookAndFeel::fancyPenColors[index]);

    releaseSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    releaseSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    releaseSlider.setRange(-10.0f, 3.0f, 0.01f);
    releaseSlider.setColour(juce::Slider::backgroundColourId, NonsenseLookAndFeel::fancyPenColors[index]);

    volumeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    volumeSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 25);
    volumeSlider.setRange(0.0f, 1.0f, 0.01f);
    volumeSlider.setColour(juce::Slider::backgroundColourId, NonsenseLookAndFeel::fancyPenColors[index]);
    volumeSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0));
    volumeSlider.setColour(juce::Slider::textBoxTextColourId, NonsenseLookAndFeel::fancyPenColors[index]);

    pitchAdjustFine.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    pitchAdjustFine.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxRight, true, 35, 25);
    pitchAdjustFine.setRange(-1.0f, 1.0f, 0.01f);
    pitchAdjustFine.setColour(juce::Slider::backgroundColourId, NonsenseLookAndFeel::fancyPenColors[index]);
    pitchAdjustFine.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0));
    pitchAdjustFine.setColour(juce::Slider::textBoxTextColourId, NonsenseLookAndFeel::fancyPenColors[index]);

    pitchAdjustRough.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    pitchAdjustRough.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxRight, true, 35, 25);
    pitchAdjustRough.setRange(-24.0f, 24.0f, 1.0f);
    pitchAdjustRough.setColour(juce::Slider::backgroundColourId, NonsenseLookAndFeel::fancyPenColors[index]);
    pitchAdjustRough.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0));
    pitchAdjustRough.setColour(juce::Slider::textBoxTextColourId, NonsenseLookAndFeel::fancyPenColors[index]);

    lowPassFreqSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    lowPassFreqSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    lowPassFreqSlider.setRange(2.0f, 32.0f, 0.01f);
    lowPassFreqSlider.setColour(juce::Slider::backgroundColourId, NonsenseLookAndFeel::fancyPenColors[index]);

    lowPassWetSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    lowPassWetSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    lowPassWetSlider.setRange(0.0f, 1.0f, 0.01f);
    lowPassWetSlider.setColour(juce::Slider::backgroundColourId, NonsenseLookAndFeel::fancyPenColors[index]);

    outOfTuneSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    outOfTuneSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    outOfTuneSlider.setRange(0.0f, 1.0f, 0.01f);
    outOfTuneSlider.setColour(juce::Slider::backgroundColourId, NonsenseLookAndFeel::fancyPenColors[index]);

    playButton.onClick = [this]()
    {
        bool isPlaying = playButton.getToggleState();
        repaint();
    };
    //playButton.setToggleState(p, juce::NotificationType::sendNotification);
    playButton.setClickingTogglesState(true);

    playButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, NonsenseLookAndFeel::fancyPenColors[index]);
    playButton.setColour(juce::TextButton::ColourIds::buttonColourId, NonsenseLookAndFeel::fancyPenColors[index]);

    addAndMakeVisible(pulseSlider);
    addAndMakeVisible(playButton);
    addAndMakeVisible(waveFormBox);

    addAndMakeVisible(attackSlider);
    addAndMakeVisible(decaySlider);
    addAndMakeVisible(sustainSlider);
    addAndMakeVisible(releaseSlider);

    addAndMakeVisible(volumeSlider);

    addAndMakeVisible(pitchAdjustRough);
    addAndMakeVisible(pitchAdjustFine);

    addAndMakeVisible(lowPassFreqSlider);
    addAndMakeVisible(lowPassWetSlider);

    addAndMakeVisible(outOfTuneSlider);

    addAndMakeVisible(graph);
    addAndMakeVisible(adsr);


    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(600, 100);
}

WaveformComponent::~WaveformComponent()
{
}

//==============================================================================
void WaveformComponent::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(playButton.getToggleState() ? NonsenseLookAndFeel::backgroundColor3 : NonsenseLookAndFeel::backgroundColor2);// getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    graph.setParameters(waveType(waveFormBox.getSelectedId()), pulseSlider.getValue(), pitchAdjustRough.getValue(), pitchAdjustFine.getValue(), volumeSlider.getValue() * playButton.getToggleState());
    graph.repaint();
    //g.setColour (juce::Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);

}

void WaveformComponent::resized()
{
    attackSlider.setBounds(0.0, getHeight() * 0.25, getWidth() * 0.05, getHeight() * 0.7);
    decaySlider.setBounds(getWidth() * 0.06, getHeight() * 0.25, getWidth() * 0.05, getHeight() * 0.7);
    sustainSlider.setBounds(getWidth() * 0.12, getHeight() * 0.25, getWidth() * 0.05, getHeight() * 0.7);
    releaseSlider.setBounds(getWidth() * 0.18, getHeight() * 0.25, getWidth() * 0.05, getHeight() * 0.7);

    //adsr.setBounds(getWidth() * 0.01, getHeight() * 0.3, getWidth() * 0.2, getHeight() * 0.65);

    pitchAdjustFine.setBounds(getWidth() * 0.25, getHeight() * 0.02, getWidth() * 0.15, getWidth() * 0.1);
    pitchAdjustRough.setBounds(getWidth() * 0.25, getHeight() * 0.02 + getWidth() * 0.085, getWidth() * 0.15, getWidth() * 0.1);

    pulseSlider.setBounds(getWidth() * 0.42, getHeight() * 0.08, getWidth() * 0.15, getHeight() * 0.55);

    graph.setBounds(getWidth() * 0.44, getHeight() * 0.6, getWidth() * 0.11, getHeight() * 0.34);

    waveFormBox.setBounds(getWidth() * 0.01, getHeight() * 0.05, getWidth() * 0.2, getHeight() * 0.2);

    outOfTuneSlider.setBounds(getWidth() * 0.555, getHeight() * 0.08, getWidth() * 0.085, getWidth() * 0.085);

    playButton.setBounds(getWidth() * 0.65, getHeight() * 0.1, getWidth() * 0.07, getHeight() * 0.8);
    volumeSlider.setBounds(getWidth() * 0.73, getHeight() * 0.05, getWidth() * 0.1, getHeight() * 0.9);

    lowPassFreqSlider.setBounds(getWidth() * 0.84, 0.0, getWidth() * 0.16, getHeight() * 0.45);
    lowPassWetSlider.setBounds(getWidth() * 0.84, getHeight() * 0.5, getWidth() * 0.16, getHeight() * 0.45);

}
