#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public AudioAppComponent, //public juce::Component,
                       private juce::Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void updateGraph();
    void timerCallback() override { updateGraph(); }
    
    
private:
    //==============================================================================
    // Your private member variables go here...
    std::unique_ptr<AudioProcessorGraph>            m_pMainGraph;
    
    
    std::unique_ptr<AudioPluginInstance>            m_ioProcOut;
    std::unique_ptr<AudioPluginInstance>            m_ioProcMidiIn;
    std::unique_ptr<AudioPluginInstance>            m_ioProcMidiOut;
    
    AudioProcessorGraph::Node::Ptr                  m_ioProcOutNode;
    AudioProcessorGraph::Node::Ptr                  m_ioProcMidiInNode;
    AudioProcessorGraph::Node::Ptr                  m_ioProcMidiOutNode;

    std::unique_ptr<AudioPluginInstance>            m_pluginInstance;
    AudioProcessorGraph::Node::Ptr                  m_pluginInstanceNode;
    AudioProcessorEditor*                           editor;
    
    AudioDeviceManager deviceManager;
    AudioProcessorPlayer player;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
