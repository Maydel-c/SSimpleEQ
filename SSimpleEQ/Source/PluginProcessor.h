/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

enum Slope
{
    Slope_12,
    Slope_24,
    Slope_36,
    Slope_48
};

struct ChainSettings
{
    float peakFreq {0}, peakGainDecibels {0}, peakQuality {0};
    float lowCutFreq {0}, highCutFreq {0};
    Slope lowCutSlope { Slope::Slope_12 }, highCutSlope { Slope::Slope_12 };
};

ChainSettings getChainSettings(juce::AudioProcessorValueTreeState& apvts);

//==============================================================================
/**
*/
class SSimpleEQAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    SSimpleEQAudioProcessor();
    ~SSimpleEQAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState apvts {*this, nullptr, "Parameters", createParameterLayout()};

private:
    
    using Filter = juce::dsp::IIR::Filter<float>;
    
    using CutFilter = juce::dsp::ProcessorChain<Filter, Filter, Filter, Filter>;
    
    using MonoChain = juce::dsp::ProcessorChain<CutFilter, Filter, CutFilter>;
    MonoChain leftChain, rightChain;
    
    enum ChainPositions
    {
        LowCut,
        Peak,
        HighCut
    };
    
    void updatePeakFilter(const ChainSettings& chainSettings);
    using Coefficients = Filter::CoefficientsPtr;
    static void updateCoefficients(Coefficients& old, const Coefficients& replacements);
    
    template<int Index, typename ChainType, typename CoefficientType>
    void update(ChainType& chain, const CoefficientType& coefficients)
    {
        updateCoefficients(chain.template get<Index>().coefficients, coefficients[Index]);
        chain.template setBypassed<Index>(false);
    }
    
    
    template<typename ChainType, typename CoefficientType>
    void updateCutFilter(ChainType& chain, // previously leftLowCut
                         const CoefficientType& coefficients, // previously cutCoefficients
//                         const ChainSettings& chainSettings) // Slope param will directly be passed in prepareToPlay and processBlock. So we don't need to get the entire chainSettings here
                        const Slope& slope) // previously lowCutSlope
    {
        chain.template setBypassed<0>(true);
        chain.template setBypassed<1>(true);
        chain.template setBypassed<2>(true);
        chain.template setBypassed<3>(true);

        switch (slope) { // previously lowCutSlope
                
            case Slope_48:
            {
                update<3>(chain, coefficients);
//                *leftLowCut.template get<3>().coefficients = *cutCoefficients[3];
//                leftLowCut.template setBypassed<3>(false);
            }
            case Slope_36:
            {
                update<2>(chain, coefficients);
            }
            case Slope_24:
            {
                update<1>(chain, coefficients);
            }
            case Slope_12:
            {
                update<0>(chain, coefficients);
            }
            default:
                break;
                
            
//            case Slope_12:
//                *leftLowCut.template get<0>().coefficients = *cutCoefficients[0];
//                leftLowCut.template setBypassed<0>(false);
//                break;
//            case Slope_24:
//                *leftLowCut.template get<0>().coefficients = *cutCoefficients[0];
//                leftLowCut.template setBypassed<0>(false);
//                *leftLowCut.template get<1>().coefficients = *cutCoefficients[1];
//                leftLowCut.template setBypassed<1>(false);
//                break;
//            case Slope_36:
//                *leftLowCut.template get<0>().coefficients = *cutCoefficients[0];
//                leftLowCut.template setBypassed<0>(false);
//                *leftLowCut.template get<1>().coefficients = *cutCoefficients[1];
//                leftLowCut.template setBypassed<1>(false);
//                *leftLowCut.template get<2>().coefficients = *cutCoefficients[2];
//                leftLowCut.template setBypassed<2>(false);
//                break;
//            case Slope_48:
//                *leftLowCut.template get<0>().coefficients = *cutCoefficients[0];
//                leftLowCut.template setBypassed<0>(false);
//                *leftLowCut.template get<1>().coefficients = *cutCoefficients[1];
//                leftLowCut.template setBypassed<1>(false);
//                *leftLowCut.template get<2>().coefficients = *cutCoefficients[2];
//                leftLowCut.template setBypassed<2>(false);
//                *leftLowCut.template get<3>().coefficients = *cutCoefficients[3];
//                leftLowCut.template setBypassed<3>(false);
//                break;
//            default:
//                break;
        }
    }
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SSimpleEQAudioProcessor)
};
