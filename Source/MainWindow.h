/*
  ==============================================================================

    MainWindow.h
    Created: 30 Jul 2021 3:59:56pm
    Author:  Sam Trolland

  ==============================================================================
*/
#pragma once

#include <JuceHeader.h>
#include "MainComponent.h"

//==============================================================================
enum ExperimentModes{
    Practice = 0,
    FirstPerformance = 1,
    SecondPerformance = 2,
    ThirdPerformance = 3
};

//==============================================================================
ApplicationCommandManager& getCommandManager();
ApplicationProperties& getAppProperties();

//==============================================================================
/*
    This class implements the desktop window that contains an instance of
    our MainComponent class.
*/
class MainWindow    : public juce::DocumentWindow,
                      public MenuBarModel
{
public:
    MainWindow (juce::String name);
    ~MainWindow() override;

    void closeButtonPressed() override;
    
    void menuBarActivated (bool isActive) override;
    StringArray getMenuBarNames() override;
    PopupMenu getMenuForIndex (int topLevelMenuIndex, const String& menuName) override;
    void menuItemSelected (int menuItemID, int topLevelMenuIndex) override;
    void showAudioSettings();
    void updateDeviceSettings();
    void addMidiInputsToMenu(PopupMenu& m);
    void addMidiOutputsToMenu(PopupMenu& m);

private:
    int experimentMode = ExperimentModes::Practice;
    Array<juce::MidiDeviceInfo> midiInputs;
    Array<juce::MidiDeviceInfo> midiOutputs;
    MainComponent *mainComponent;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
};
