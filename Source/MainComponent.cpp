#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent(): m_pMainGraph (new AudioProcessorGraph())
{
    auto midiList = juce::MidiInput::getAvailableDevices();
    
    auto inputDevice  = midiList[1];    //juce::MidiInput::getDefaultDevice();
    
    auto outputDevice = juce::MidiOutput::getDefaultDevice();
    
    m_pMainGraph->enableAllBuses();
    
    deviceManager.initialiseWithDefaultDevices(2, 2);
    deviceManager.addAudioCallback(&player);
    deviceManager.setMidiInputDeviceEnabled(inputDevice.identifier, true);
    deviceManager.addMidiInputDeviceCallback(inputDevice.identifier, &player);
    deviceManager.setDefaultMidiOutputDevice(outputDevice.identifier);
    
    //initialiseGraph();
    
    //m_pMainGraph->setPlayConfigDetails (2, 2, 44100.0, 512);
    
    m_ioProcOut         = std::make_unique<AudioProcessorGraph::AudioGraphIOProcessor> (AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);
    m_ioProcMidiIn      = std::make_unique<AudioProcessorGraph::AudioGraphIOProcessor> (AudioProcessorGraph::AudioGraphIOProcessor::midiInputNode);
    m_ioProcMidiOut     = std::make_unique<AudioProcessorGraph::AudioGraphIOProcessor> (AudioProcessorGraph::AudioGraphIOProcessor::midiOutputNode);
    
    //AudioPluginInstance * instance = new AudioProcessorGraph::AudioGraphIOProcessor (AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);
    
    //std::unique_ptr<AudioProcessor> ap = new AudioProcessorGraph::AudioGraphIOProcessor (AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);
    //std::unique_ptr<AudioPluginInstance> instance = std::make_unique<AudioProcessorGraph::AudioGraphIOProcessor> (AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode);
    
    m_ioProcOutNode     = m_pMainGraph->addNode (std::move (m_ioProcOut) );
    m_ioProcMidiInNode  = m_pMainGraph->addNode (std::move (m_ioProcMidiIn) );
    m_ioProcMidiOutNode = m_pMainGraph->addNode (std::move (m_ioProcMidiOut) );
    
    m_ioProcOutNode->getProcessor()->enableAllBuses();
    
    OwnedArray<PluginDescription> pluginDescriptions;
    KnownPluginList plist;
    AudioPluginFormatManager pluginFormatManager;
    pluginFormatManager.addDefaultFormats();
    for (int i = 0; i < pluginFormatManager.getNumFormats(); ++i)
    {
        plist.scanAndAddFile("/Users/Sam/Documents/Research/TeresaProjects/ML_Music/VST-HOST-dinverno/extras/AudioPluginHost/Plugins/Upright Piano.vst3", true, pluginDescriptions,
                             *pluginFormatManager.getFormat(i));
    }
    jassert (pluginDescriptions.size() > 0);
    String msg ("Oh no!");
    m_pluginInstance = pluginFormatManager.createPluginInstance(*pluginDescriptions[0], 44100.0, 512, msg);

    
    
    editor = m_pluginInstance->createEditor();
    auto bc = editor->getConstrainer();
    editor->setBounds(0, 0, bc->getMinimumWidth(), bc->getMinimumHeight());
    addAndMakeVisible (editor);
    

    m_pluginInstanceNode = m_pMainGraph->addNode (std::move (m_pluginInstance) );
    m_pluginInstanceNode->getProcessor()->enableAllBuses();
    //addAndMakeVisible(&graphHolder);
    
    m_pMainGraph->addConnection({ {m_ioProcMidiInNode->nodeID, juce::AudioProcessorGraph::midiChannelIndex},
                                  {m_pluginInstanceNode->nodeID, juce::AudioProcessorGraph::midiChannelIndex}
                                });
    
    m_pMainGraph->addConnection({ {m_pluginInstanceNode->nodeID, 0},
                                  {m_ioProcOutNode->nodeID, 0}
                                });
    
    m_pMainGraph->addConnection({ {m_pluginInstanceNode->nodeID, 1},
                                  {m_ioProcOutNode->nodeID, 1}
                                });
    
    player.setProcessor (m_pMainGraph.get());
    
    setSize (1024, 768);
    startTimer (100);
}

MainComponent::~MainComponent()
{
    auto device = juce::MidiInput::getDefaultDevice();
    
    deviceManager.removeAudioCallback (&player);
    deviceManager.setMidiInputDeviceEnabled (device.identifier, false);
    deviceManager.removeMidiInputDeviceCallback (device.identifier, &player);
    
}
//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.
    
    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.
    
    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!
    
    // For more details, see the help for AudioProcessor::getNextAudioBlock()
    
    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    
    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
    
    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::Font (16.0f));
    g.setColour (juce::Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

void MainComponent::updateGraph()
{
    /*for (auto slot : slots)
    {
        if (slot != nullptr)
        {
            activeSlots.add (slot);
            
            slot->getProcessor()->setPlayConfigDetails (mainProcessor->getMainBusNumInputChannels(),
                                                        mainProcessor->getMainBusNumOutputChannels(),
                                                        mainProcessor->getSampleRate(),
                                                        mainProcessor->getBlockSize());
        }
    }*/
}
