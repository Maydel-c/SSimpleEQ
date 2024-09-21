/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SSimpleEQAudioProcessor::SSimpleEQAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

SSimpleEQAudioProcessor::~SSimpleEQAudioProcessor()
{
}

//==============================================================================
const juce::String SSimpleEQAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SSimpleEQAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SSimpleEQAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SSimpleEQAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SSimpleEQAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SSimpleEQAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SSimpleEQAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SSimpleEQAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SSimpleEQAudioProcessor::getProgramName (int index)
{
    return {};
}

void SSimpleEQAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SSimpleEQAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = 1;
    
    leftChain.prepare(spec);
    rightChain.prepare(spec);
    
    updateFilters();
//    auto chainSettings = getChainSettings(apvts);
//
//    updatePeakFilter(chainSettings);
//
//    // LOWCUT - GET COEFFICIENTS AND UPDATE FILTER PARAMS
//    auto lowCutCoefficients = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(chainSettings.lowCutFreq,
//                                                                                                        sampleRate,
//                                                                                                          2*(chainSettings.lowCutSlope + 1));
//    auto& leftLowCut = leftChain.get<ChainPositions::LowCut>();
//    updateCutFilter(leftLowCut, lowCutCoefficients, chainSettings.lowCutSlope);
//
//    auto& rightLowCut = rightChain.get<ChainPositions::LowCut>();
//    updateCutFilter(rightLowCut, lowCutCoefficients, chainSettings.lowCutSlope);
//
//    // HIGHCUT - GET COEFFICIENTS AND UPDATE FILTER PARAMS
//    auto highCutCoefficients = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(chainSettings.highCutFreq,
//                                                                                                          sampleRate,
//                                                                                                          2*(chainSettings.highCutSlope + 1));
//
//    auto& leftHighCut = leftChain.get<ChainPositions::HighCut>();
//    updateCutFilter(leftHighCut, highCutCoefficients, chainSettings.highCutSlope);
//
//    auto& rightHighCut = rightChain.get<ChainPositions::HighCut>();
//    updateCutFilter(rightHighCut, highCutCoefficients, chainSettings.highCutSlope);
}

void SSimpleEQAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SSimpleEQAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
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

void SSimpleEQAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    updateFilters();
    
//    auto chainSettings = getChainSettings(apvts);
    
//    updatePeakFilter(chainSettings);

//    // LOWCUT - GET COEFFICIENTS AND UPDATE FILTER PARAMS
//    auto lowCutCoefficients = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(chainSettings.lowCutFreq,
//                                                                                                       getSampleRate(),
//                                                                                                       2*(chainSettings.lowCutSlope + 1));
//
//    auto& leftLowCut = leftChain.get<ChainPositions::LowCut>();
//    updateCutFilter(leftLowCut, lowCutCoefficients, chainSettings.lowCutSlope);
//
//    auto& rightLowCut = rightChain.get<ChainPositions::LowCut>();
//    updateCutFilter(rightLowCut, lowCutCoefficients, chainSettings.lowCutSlope);
    
//    // HIGHCUT - GET COEFFICIENTS AND UPDATE FILTER PARAMS
//    auto highCutCoefficients = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(chainSettings.highCutFreq,
//                                                                                                          getSampleRate(),
//                                                                                                          2*(chainSettings.highCutSlope + 1));
//    auto& leftHighCut = leftChain.get<ChainPositions::HighCut>();
//    updateCutFilter(leftHighCut, highCutCoefficients, chainSettings.highCutSlope);
//
//    auto& rightHighCut = rightChain.get<ChainPositions::HighCut>();
//    updateCutFilter(rightHighCut, highCutCoefficients, chainSettings.highCutSlope);
//
    juce::dsp::AudioBlock<float> block (buffer);
    auto leftBlock = block.getSingleChannelBlock(0);
    auto rightBlock = block.getSingleChannelBlock(1);
    
    juce::dsp::ProcessContextReplacing<float> leftContext (leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext (rightBlock);
    
    leftChain.process(leftContext);
    rightChain.process(rightContext);
}

//==============================================================================
bool SSimpleEQAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SSimpleEQAudioProcessor::createEditor()
{
//    return new SSimpleEQAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void SSimpleEQAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SSimpleEQAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

ChainSettings getChainSettings(juce::AudioProcessorValueTreeState& apvts)
{
    ChainSettings settings;
    
    settings.lowCutFreq = apvts.getRawParameterValue("LowCut Freq")->load();
    settings.highCutFreq = apvts.getRawParameterValue("HighCut Freq")->load();
    settings.peakFreq = apvts.getRawParameterValue("Peak Freq")->load();
    settings.peakGainDecibels = apvts.getRawParameterValue("Peak Gain")->load();
    settings.peakQuality = apvts.getRawParameterValue("Peak Quality")->load();
    settings.lowCutSlope = static_cast<Slope>(apvts.getRawParameterValue("LowCut Slope")->load());
    settings.highCutSlope = static_cast<Slope>(apvts.getRawParameterValue("HighCut Slope")->load());
    
    return settings;
}


void SSimpleEQAudioProcessor::updatePeakFilter(const ChainSettings& chainSettings)
{
    auto peakCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), chainSettings.peakFreq, chainSettings.peakQuality, juce::Decibels::decibelsToGain(chainSettings.peakGainDecibels));

    updateCoefficients(leftChain.get<ChainPositions::Peak>().coefficients, peakCoefficients);
    updateCoefficients(rightChain.get<ChainPositions::Peak>().coefficients, peakCoefficients);
}

void SSimpleEQAudioProcessor::updateLowCutFilters(const ChainSettings& chainSettings)
{
    // LOWCUT - GET COEFFICIENTS AND UPDATE FILTER PARAMS
    auto lowCutCoefficients = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(chainSettings.lowCutFreq,
                                                                                                       getSampleRate(),
                                                                                                       2*(chainSettings.lowCutSlope + 1));

    auto& leftLowCut = leftChain.get<ChainPositions::LowCut>();
    updateCutFilter(leftLowCut, lowCutCoefficients, chainSettings.lowCutSlope);

    auto& rightLowCut = rightChain.get<ChainPositions::LowCut>();
    updateCutFilter(rightLowCut, lowCutCoefficients, chainSettings.lowCutSlope);
}

void SSimpleEQAudioProcessor::updateHighCutFilters(const ChainSettings &chainSettings)
{
    // HIGHCUT - GET COEFFICIENTS AND UPDATE FILTER PARAMS
    auto highCutCoefficients = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(chainSettings.highCutFreq,
                                                                                                          getSampleRate(),
                                                                                                          2*(chainSettings.highCutSlope + 1));
    auto& leftHighCut = leftChain.get<ChainPositions::HighCut>();
    updateCutFilter(leftHighCut, highCutCoefficients, chainSettings.highCutSlope);
    
    auto& rightHighCut = rightChain.get<ChainPositions::HighCut>();
    updateCutFilter(rightHighCut, highCutCoefficients, chainSettings.highCutSlope);
}

void SSimpleEQAudioProcessor::updateFilters()
{
    auto chainSettings = getChainSettings(apvts);
    updateLowCutFilters(chainSettings);
    updatePeakFilter(chainSettings);
    updateHighCutFilters(chainSettings);
}

void SSimpleEQAudioProcessor::updateCoefficients(Coefficients& old, const Coefficients& replacements)
{
    *old = *replacements;
}


juce::AudioProcessorValueTreeState::ParameterLayout SSimpleEQAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"LowCut Freq", 1},
                                                           "LowCut Freq",
                                                           juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.25f),
                                                           20.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"HighCut Freq", 1},
                                                           "HighCut Freq",
                                                           juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.25f),
                                                           20000.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"Peak Freq", 1},
                                                           "Peak Freq",
                                                           juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.25f),
                                                           750.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"Peak Gain", 1},
                                                           "Peak Gain",
                                                           juce::NormalisableRange<float>(-24.f, 24.f, 0.5f, 1.f),
                                                           0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"Peak Quality", 1},
                                                           "Peak Quality",
                                                           juce::NormalisableRange<float>(0.1f, 10.f, 0.05f, 1.f),
                                                           0));
    
    juce::StringArray stringArray;
    for(auto i=0; i<4; ++i)
    {
        juce::String str;
        str << (12 + i*(12));
        str << "db/Oct";
        stringArray.add(str);
    }
    
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{"LowCut Slope", 1},
                                                            "LowCut Slope",
                                                            stringArray,
                                                            0));
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{"HighCut Slope", 1},
                                                            "HighCut Slope",
                                                            stringArray,
                                                            0));
    
    
    return layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SSimpleEQAudioProcessor();
}
