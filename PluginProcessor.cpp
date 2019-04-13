/*
  ==============================================================================

	This file was auto-generated!

	It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FlangerAudioProcessor::FlangerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif
	)
#endif
{
	addParameter(drywet = new AudioParameterFloat("drywet", "dry wet", 0, 1.0, .5));
	addParameter(feedback = new AudioParameterFloat("feedback", "feedback", 0, .9, .5));
	addParameter(rate = new AudioParameterFloat("rate", "rate", .1f, 20.f, 10.f));
	addParameter(depth = new AudioParameterFloat("depth", "depth", 0, 1, .5));
	addParameter(offset = new AudioParameterFloat("offset", "offset", 0.0f, 1.f, 0.f));
	mWritePosition = 0;
	mlfo = 0;
}

FlangerAudioProcessor::~FlangerAudioProcessor()
{

}

//==============================================================================
const String FlangerAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool FlangerAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool FlangerAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool FlangerAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double FlangerAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

float linit(float x, float x1, float phase) {
	return (1 - phase)*x + phase * x1;
}

int FlangerAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
				// so this should be at least 1, even if you're not really implementing programs.
}

int FlangerAudioProcessor::getCurrentProgram()
{
	return 0;
}

void FlangerAudioProcessor::setCurrentProgram(int index)
{
}

const String FlangerAudioProcessor::getProgramName(int index)
{
	return {};
}

void FlangerAudioProcessor::changeProgramName(int index, const String& newName)
{
}

//==============================================================================
void FlangerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	float fs = sampleRate;
	DelaybufferLength = (int)fs * 2;
	mWritePosition = 0;

	mDelayBufferLeft.setSize(1, DelaybufferLength, false, true);
	mDelayBufferRight.setSize(1, DelaybufferLength, false, true);
	mDelayBufferLeft.clear();
	mDelayBufferRight.clear();
	feedbackleft = 0;
	feedbackright = 0;
	mlfo = 0;
}

void FlangerAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FlangerAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void FlangerAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();


	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());
	const int bufferlength = buffer.getNumSamples();
	float* leftChannel = buffer.getWritePointer(0);
	float* rightChannel = buffer.getWritePointer(1);

	for (int sample = 0;sample < bufferlength; ++sample)
	{
		float lfout = sin(2 * double_Pi*mlfo);
		mlfo += *rate / getSampleRate();
		if (mlfo > 1) {
			mlfo -= 1;
		}
		lfout *= *depth;
		float rlfo = mlfo + *offset;
		if (rlfo > 1) {
			rlfo -= 1;
		}
		float lfmap = jmap(lfout, -1.f, 1.f, .001f, .005f);
		float rfout = sin(2 * double_Pi*rlfo);
		float rfmap = jmap(rfout, -1.f, 1.f, .001f, .005f);
		mDelayBufferLeft.setSample(0, mWritePosition, leftChannel[sample] + feedbackleft);
		mDelayBufferRight.setSample(0, mWritePosition, rightChannel[sample] + feedbackright);

		float mReadPosition = mWritePosition - lfmap * getSampleRate();
		if (mReadPosition < 0) {
			mReadPosition += DelaybufferLength;
		}
		int rint1 = (int)mReadPosition;
		float phas1 = mReadPosition - rint1;
		int rint2 = rint1 + 1;
		if (rint2 >= DelaybufferLength) {
			rint2 -= DelaybufferLength;
		}

		float rReadPosition = mWritePosition - rfmap * getSampleRate();
		if (rReadPosition < 0) {
			rReadPosition += DelaybufferLength;
		}
		int rint3 = (int)rReadPosition;
		float phas2 = rReadPosition - rint3;
		int rint4 = rint3 + 1;
		if (rint4 >= DelaybufferLength) {
			rint4 -= DelaybufferLength;
		}


		float delayleft = (1 - phas1)*mDelayBufferLeft.getSample(0, rint1) + phas1 * mDelayBufferLeft.getSample(0, rint2);
		float delayright = (1 - phas2)*mDelayBufferRight.getSample(0, rint3) + phas2 * mDelayBufferRight.getSample(0, rint4);

		feedbackleft = *feedback*delayleft;
		feedbackright = *feedback*delayright;
		float outl = (1 - *drywet)*buffer.getSample(0, sample) + *drywet*feedbackleft;
		float outr = (1 - *drywet)*buffer.getSample(1, sample) + *drywet*feedbackright;
		buffer.addSample(0, sample, outl);
		buffer.addSample(1, sample, outr);
		mWritePosition++;
		if (mWritePosition > DelaybufferLength) {
			mWritePosition = 0;
		}
	}
}

//==============================================================================
bool FlangerAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* FlangerAudioProcessor::createEditor()
{
	return new FlangerAudioProcessorEditor(*this);
}

//==============================================================================
void FlangerAudioProcessor::getStateInformation(MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void FlangerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new FlangerAudioProcessor();
}
