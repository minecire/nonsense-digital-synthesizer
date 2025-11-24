/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NonsenseAudioProcessor::NonsenseAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ), state(*this, nullptr, "parameterState", createParameters())
#endif
{
}

NonsenseAudioProcessor::~NonsenseAudioProcessor()
{
}

//==============================================================================
const juce::String NonsenseAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NonsenseAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool NonsenseAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool NonsenseAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double NonsenseAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NonsenseAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int NonsenseAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NonsenseAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String NonsenseAudioProcessor::getProgramName(int index)
{
    return {};
}

void NonsenseAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void NonsenseAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    waves.resize(numWaves);
    playParams.resize(numWaves);
    pulseParams.resize(numWaves);
    waveParams.resize(numWaves);

    attackParams.resize(numWaves);
    decayParams.resize(numWaves);
    sustainParams.resize(numWaves);
    releaseParams.resize(numWaves);

    volumeParams.resize(numWaves);

    roughPitchParams.resize(numWaves);
    finePitchParams.resize(numWaves);

    lowPassWetParams.resize(numWaves);
    lowPassFreqParams.resize(numWaves);

    outOfTuneParams.resize(numWaves);

    for (int i = 0; i < numWaves; i++) {
        waves[i].prepare(sampleRate, getTotalNumOutputChannels());
        playParams[i] = state.getRawParameterValue("Play" + std::to_string(i));
        pulseParams[i] = state.getRawParameterValue("Pulse" + std::to_string(i));
        waveParams[i] = state.getRawParameterValue("WaveType" + std::to_string(i));

        attackParams[i] = state.getRawParameterValue("Attack" + std::to_string(i));
        decayParams[i] = state.getRawParameterValue("Decay" + std::to_string(i));
        sustainParams[i] = state.getRawParameterValue("Sustain" + std::to_string(i));
        releaseParams[i] = state.getRawParameterValue("Release" + std::to_string(i));

        volumeParams[i] = state.getRawParameterValue("Volume" + std::to_string(i));

        roughPitchParams[i] = state.getRawParameterValue("PitchR" + std::to_string(i));
        finePitchParams[i] = state.getRawParameterValue("PitchF" + std::to_string(i));

        lowPassWetParams[i] = state.getRawParameterValue("LowPW" + std::to_string(i));
        lowPassFreqParams[i] = state.getRawParameterValue("LowPF" + std::to_string(i));

        outOfTuneParams[i] = state.getRawParameterValue("OutOfTune" + std::to_string(i));
    }
}

void NonsenseAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NonsenseAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void NonsenseAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.


    //sineWave.set_frequency(frequencyParam->load());

    
    for (int i = 0; i < numWaves; i++) {
        waves[i].set_amplitude(volumeParams[i]->load() * playParams[i]->load());

        waves[i].set_pulse(pulseParams[i]->load());

        float waveFlt = waveParams[i]->load();
        waves[i].set_wave_type((waveType)((int)waveFlt + 1));

        waves[i].set_adsr(attackParams[i]->load(), decayParams[i]->load(), sustainParams[i]->load(), releaseParams[i]->load());

        waves[i].set_pitch_shift(roughPitchParams[i]->load() + finePitchParams[i]->load());

        waves[i].set_low_pass(lowPassFreqParams[i]->load(), lowPassWetParams[i]->load());

        waves[i].set_out_of_tune(outOfTuneParams[i]->load());

        waves[i].process(buffer, midiMessages);
    }
}

//==============================================================================
bool NonsenseAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NonsenseAudioProcessor::createEditor()
{
    return new NonsenseAudioProcessorEditor(*this);
}

//==============================================================================
void NonsenseAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    juce::XmlElement data = *state.state.createXml();
    std::cout << data.toString();
    destData.reset();
    copyXmlToBinary(data, destData);
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void NonsenseAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlE = getXmlFromBinary(data, sizeInBytes);
    std::cout << (*xmlE).toString() << '\n';
    if (xmlE != nullptr) {
        if (xmlE->hasTagName(state.state.getType()))
            state.replaceState(juce::ValueTree::fromXml(*xmlE));
    }

    waveformBoxPositions.resize(10);

    for (int i = 0; i < 10; i++) {
        waveformBoxPositions[i] = 1.0;
    }

    for (auto* child : (*xmlE).getChildIterator()) {
        juce::String id = child->getStringAttribute("id");
        if (id.startsWith("WaveType")) {
            int index = id[8] - '0';
            if(index >= 0 && index < 10)
                waveformBoxPositions[index] = child->getDoubleAttribute("value", 0.0) + 1.0;
        }
    }

    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NonsenseAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout NonsenseAudioProcessor::createParameters()
{
    return {
        std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ "Play0" }, "Playing", true),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Pulse0" }, "Pulse Width", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{ "WaveType0" }, "Wave Type", juce::StringArray{"Sine", "Square", "Triangle", "Saw", "Noise"}, 1),
        std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ "Play1" }, "Playing", false),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Pulse1" }, "Pulse Width", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{ "WaveType1" }, "Wave Type", juce::StringArray{"Sine", "Square", "Triangle", "Saw", "Noise"}, 1),
        std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ "Play2" }, "Playing", false),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Pulse2" }, "Pulse Width", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{ "WaveType2" }, "Wave Type", juce::StringArray{"Sine", "Square", "Triangle", "Saw", "Noise"}, 1),
        std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ "Play3" }, "Playing", false),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Pulse3" }, "Pulse Width", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{ "WaveType3" }, "Wave Type", juce::StringArray{"Sine", "Square", "Triangle", "Saw", "Noise"}, 1),
        std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ "Play4" }, "Playing", false),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Pulse4" }, "Pulse Width", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{ "WaveType4" }, "Wave Type", juce::StringArray{"Sine", "Square", "Triangle", "Saw", "Noise"}, 1),
        std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ "Play5" }, "Playing", false),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Pulse5" }, "Pulse Width", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{ "WaveType5" }, "Wave Type", juce::StringArray{"Sine", "Square", "Triangle", "Saw", "Noise"}, 1),
        std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ "Play6" }, "Playing", false),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Pulse6" }, "Pulse Width", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{ "WaveType6" }, "Wave Type", juce::StringArray{"Sine", "Square", "Triangle", "Saw", "Noise"}, 1),
        std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ "Play7" }, "Playing", false),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Pulse7" }, "Pulse Width", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{ "WaveType7" }, "Wave Type", juce::StringArray{"Sine", "Square", "Triangle", "Saw", "Noise"}, 1),
        std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ "Play8" }, "Playing", false),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Pulse8" }, "Pulse Width", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{ "WaveType8" }, "Wave Type", juce::StringArray{"Sine", "Square", "Triangle", "Saw", "Noise"}, 1),
        std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ "Play9" }, "Playing", false),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Pulse9" }, "Pulse Width", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{ "WaveType9" }, "Wave Type", juce::StringArray{"Sine", "Square", "Triangle", "Saw", "Noise"}, 1),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Attack0" }, "Attack", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Attack1" }, "Attack", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Attack2" }, "Attack", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Attack3" }, "Attack", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Attack4" }, "Attack", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Attack5" }, "Attack", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Attack6" }, "Attack", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Attack7" }, "Attack", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Attack8" }, "Attack", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Attack9" }, "Attack", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Decay0" }, "Decay", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Decay1" }, "Decay", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Decay2" }, "Decay", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Decay3" }, "Decay", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Decay4" }, "Decay", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Decay5" }, "Decay", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Decay6" }, "Decay", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Decay7" }, "Decay", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Decay8" }, "Decay", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Decay9" }, "Decay", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Sustain0" }, "Sustain", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Sustain1" }, "Sustain", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Sustain2" }, "Sustain", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Sustain3" }, "Sustain", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Sustain4" }, "Sustain", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Sustain5" }, "Sustain", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Sustain6" }, "Sustain", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Sustain7" }, "Sustain", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Sustain8" }, "Sustain", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Sustain9" }, "Sustain", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Release0" }, "Release", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Release1" }, "Release", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Release2" }, "Release", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Release3" }, "Release", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Release4" }, "Release", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Release5" }, "Release", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Release6" }, "Release", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Release7" }, "Release", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Release8" }, "Release", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Release9" }, "Release", -10.0f, 3.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Volume0" }, "Volume", 0.0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Volume1" }, "Volume", 0.0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Volume2" }, "Volume", 0.0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Volume3" }, "Volume", 0.0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Volume4" }, "Volume", 0.0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Volume5" }, "Volume", 0.0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Volume6" }, "Volume", 0.0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Volume7" }, "Volume", 0.0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Volume8" }, "Volume", 0.0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Volume9" }, "Volume", 0.0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "PitchR0" }, "Pitch Rough", -24.0f, 24.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "PitchR1" }, "Pitch Rough", -24.0f, 24.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "PitchR2" }, "Pitch Rough", -24.0f, 24.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "PitchR3" }, "Pitch Rough", -24.0f, 24.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "PitchR4" }, "Pitch Rough", -24.0f, 24.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "PitchR5" }, "Pitch Rough", -24.0f, 24.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "PitchR6" }, "Pitch Rough", -24.0f, 24.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "PitchR7" }, "Pitch Rough", -24.0f, 24.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "PitchR8" }, "Pitch Rough", -24.0f, 24.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "PitchR9" }, "Pitch Rough", -24.0f, 24.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "PitchF0" }, "Pitch Fine", -1.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "PitchF1" }, "Pitch Fine", -1.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "PitchF2" }, "Pitch Fine", -1.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "PitchF3" }, "Pitch Fine", -1.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "PitchF4" }, "Pitch Fine", -1.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "PitchF5" }, "Pitch Fine", -1.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "PitchF6" }, "Pitch Fine", -1.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "PitchF7" }, "Pitch Fine", -1.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "PitchF8" }, "Pitch Fine", -1.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "PitchF9" }, "Pitch Fine", -1.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "LowPF0" }, "Low Pass Freq", 1.0f, 32.0f, 2.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "LowPF1" }, "Low Pass Freq", 1.0f, 32.0f, 2.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "LowPF2" }, "Low Pass Freq", 1.0f, 32.0f, 2.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "LowPF3" }, "Low Pass Freq", 1.0f, 32.0f, 2.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "LowPF4" }, "Low Pass Freq", 1.0f, 32.0f, 2.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "LowPF5" }, "Low Pass Freq", 1.0f, 32.0f, 2.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "LowPF6" }, "Low Pass Freq", 1.0f, 32.0f, 2.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "LowPF7" }, "Low Pass Freq", 1.0f, 32.0f, 2.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "LowPF8" }, "Low Pass Freq", 1.0f, 32.0f, 2.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "LowPF9" }, "Low Pass Freq", 1.0f, 32.0f, 2.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "LowPW0" }, "Low Pass Wet", -0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "LowPW1" }, "Low Pass Wet", -0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "LowPW2" }, "Low Pass Wet", -0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "LowPW3" }, "Low Pass Wet", -0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "LowPW4" }, "Low Pass Wet", -0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "LowPW5" }, "Low Pass Wet", -0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "LowPW6" }, "Low Pass Wet", -0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "LowPW7" }, "Low Pass Wet", -0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "LowPW8" }, "Low Pass Wet", -0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "LowPW9" }, "Low Pass Wet", -0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "OutOfTune0" }, "Out of Tune", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "OutOfTune1" }, "Out of Tune", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "OutOfTune2" }, "Out of Tune", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "OutOfTune3" }, "Out of Tune", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "OutOfTune4" }, "Out of Tune", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "OutOfTune5" }, "Out of Tune", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "OutOfTune6" }, "Out of Tune", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "OutOfTune7" }, "Out of Tune", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "OutOfTune8" }, "Out of Tune", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "OutOfTune9" }, "Out of Tune", 0.0f, 1.0f, 0.0f)
    };
}