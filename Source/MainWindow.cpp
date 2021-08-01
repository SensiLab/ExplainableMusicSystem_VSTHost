/*
  ==============================================================================

    MainWindow.cpp
    Created: 30 Jul 2021 3:59:56pm
    Author:  Sam Trolland

  ==============================================================================
*/

#include "MainWindow.h"

//==============================================================================

MainWindow::MainWindow (juce::String name)
    : DocumentWindow (name,
                      juce::Desktop::getInstance().getDefaultLookAndFeel()
                                                  .findColour (juce::ResizableWindow::backgroundColourId),
                      DocumentWindow::allButtons)
{
    //setUsingNativeTitleBar (true);
    mainComponent = new MainComponent();
    setContentOwned (mainComponent, true);
    
   #if JUCE_IOS || JUCE_ANDROID
    setFullScreen (true);
   #else
    setResizable (false, false);
    centreWithSize (getWidth(), getHeight());
   #endif

    setVisible (true);
    
    #if JUCE_IOS || JUCE_ANDROID
      graphHolder->burgerMenu.setModel (this);
    #else
     #if JUCE_MAC
      setMacMainMenu (this);
     #else
      setMenuBar (this);
     #endif
    #endif
}

MainWindow::~MainWindow()
{
    getAppProperties().getUserSettings()->setValue ("mainWindowPos", getWindowStateAsString());
    clearContentComponent();

  #if ! (JUCE_ANDROID || JUCE_IOS)
   #if JUCE_MAC
    setMacMainMenu (nullptr);
   #else
    setMenuBar (nullptr);
   #endif
  #endif

}

void MainWindow::closeButtonPressed()
{
    // This is called when the user tries to close this window. Here, we'll just
    // ask the app to quit when this happens, but you can change this to do
    // whatever you need.
    JUCEApplication::getInstance()->systemRequestedQuit();
}

void MainWindow::menuBarActivated (bool isActive)
{
    
}

StringArray MainWindow::getMenuBarNames()
{
    // MenuBar Items
    
    StringArray names;
    //names.add ("File");
    //names.add ("Plugins");
    //names.add ("Options");
    //names.add ("Windows");
    names.add ("Experiment");
    names.add ("Configuration");
    return names;
}

PopupMenu MainWindow::getMenuForIndex (int topLevelMenuIndex, const String& menuName)
{
    // Add items to Menu
    
    PopupMenu menu;

    if (topLevelMenuIndex == 0) {
        // "Experiment" Menu
        
        // Create Menu Items
        PopupMenu::Item *item0 = new PopupMenu::Item("Practice");
        PopupMenu::Item *item1 = new PopupMenu::Item("First Performance");
        PopupMenu::Item *item2 = new PopupMenu::Item("Second Performance");
        PopupMenu::Item *item3 = new PopupMenu::Item("Third Performance");
        
        // Add IDs to Items
        item0->setID(200);
        item1->setID(201);
        item2->setID(202);
        item3->setID(203);
        
        // Handle Experiment Mode Selection
        switch(experimentMode){
            case ExperimentModes::Practice:
                item0->setTicked();
                break;
            case ExperimentModes::FirstPerformance:
                item1->setTicked();
                break;
            case ExperimentModes::SecondPerformance:
                item2->setTicked();
                break;
            case ExperimentModes::ThirdPerformance:
                item3->setTicked();
                break;
            default:
                break;
        }

        // Add Items to Menu
        menu.addItem(*item0);
        menu.addItem(*item1);
        menu.addItem(*item2);
        menu.addItem(*item3);
        
    }
    if (topLevelMenuIndex == 1) {
        // "Configuration" Menu
        PopupMenu midiInputMenu;
        addMidiInputsToMenu(midiInputMenu);
        menu.addSubMenu("Select Midi Inputs",midiInputMenu);
        
        PopupMenu midiOutputMenu;
        addMidiOutputsToMenu(midiOutputMenu);
        menu.addSubMenu("Select Midi Outputs",midiOutputMenu);
    }

    return menu;
}

void MainWindow::menuItemSelected (int menuItemID, int topLevelMenuIndex)
{
    // Handle Menu Selection
    if (menuItemID == 200)
    {
        // Performance 1: Update plugins
        experimentMode = ExperimentModes::Practice;
        mainComponent->setExperimentMode(experimentMode);       // Update MainComponent
    }else if (menuItemID == 201)
    {
        // Performance 3: Update plugins
        experimentMode = ExperimentModes::FirstPerformance;
        mainComponent->setExperimentMode(experimentMode);       // Update MainComponent
    }else if (menuItemID == 202)
    {
        // Performance 2: Update plugins
        experimentMode = ExperimentModes::SecondPerformance;
        mainComponent->setExperimentMode(experimentMode);       // Update MainComponent
    }else if (menuItemID == 203)
    {
        // Performance 3: Update plugins
        experimentMode = ExperimentModes::ThirdPerformance;
        mainComponent->setExperimentMode(experimentMode);       // Update MainComponent
    }else if (300 <= menuItemID && menuItemID < 400){
        //Midi Input Device Selected
        int midiDeviceIndex = menuItemID- 300;
        mainComponent->toggleMidiInputOnOff(midiInputs[midiDeviceIndex].identifier);
    }else if (400 <= menuItemID && menuItemID < 500){
        //Midi Output Device Selected
        int midiDeviceIndex = menuItemID- 400;
        mainComponent->setMidiOutputDevice(midiOutputs[midiDeviceIndex].identifier);
    }
    
    
    
    // Update Menu Item Selection
    menuItemsChanged();
}

void MainWindow::addMidiInputsToMenu(PopupMenu& m)
{
    AudioDeviceManager* deviceManager = mainComponent->getDeviceManager();
    
    midiInputs = juce::MidiInput::getAvailableDevices();
    
    for(int i = 0; i < midiInputs.size(); i++){
        // Get Midi Device Info
        auto inputDevice  = midiInputs[i];    //juce::MidiInput::getDefaultDevice();
        bool enabled = deviceManager->isMidiInputDeviceEnabled(inputDevice.identifier);
        
        // Add Midi Device to menu
        PopupMenu::Item *item = new PopupMenu::Item(inputDevice.name);
        item->setID(300+i);
        if (enabled){
            item->setTicked();
        }
        m.addItem(*item);
    }
}

void MainWindow::addMidiOutputsToMenu(PopupMenu& m)
{
    AudioDeviceManager* deviceManager = mainComponent->getDeviceManager();
    
    midiOutputs = juce::MidiOutput::getAvailableDevices();
    juce::String outputMidiDeviceIdentifier = mainComponent->getMidiOutputDeviceIdentifier();
    
    for(int i = 0; i < midiOutputs.size(); i++){
        // Get Midi Device Info
        auto outputDevice  = midiOutputs[i];    //juce::MidiInput::getDefaultDevice();
        bool enabled = false;
        if (outputDevice.identifier == outputMidiDeviceIdentifier){
            enabled = true;
        }
        
        // Add Midi Device to menu
        PopupMenu::Item *item = new PopupMenu::Item(outputDevice.name);
        item->setID(400+i);
        if (enabled){
            item->setTicked();
        }
        m.addItem(*item);
    }
}

void MainWindow::updateDeviceSettings()
{
    //deviceManager.
    //mainComponent->updateDeviceSettings(this->deviceManager.);
}
