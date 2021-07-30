/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainComponent.h"
#include "MainWindow.h"



//==============================================================================
class VSTSimpleHostApplication  : public juce::JUCEApplication
{
public:
    //==============================================================================
    VSTSimpleHostApplication() {}

    const juce::String getApplicationName() override       { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override             { return true; }
    std::unique_ptr<ApplicationProperties> appProperties;
    
    //==============================================================================
    void initialise (const juce::String& commandLine) override
    {
        // This method is where you should put your application's initialisation code..

        PropertiesFile::Options options;
        options.applicationName     = "DinvernoSystemLite VST Host";
        options.filenameSuffix      = "settings";
        options.osxLibrarySubFolder = "Preferences";

        appProperties.reset (new ApplicationProperties());
        appProperties->setStorageParameters (options);
        
        mainWindow.reset (new MainWindow (getApplicationName()));
        
        //mainWindow->setUsingNativeTitleBar (true); // Done in mainWindow
        mainWindow->menuItemsChanged();
        
       // triggerAsyncUpdate();
    }

    void shutdown() override
    {
        // Add your application's shutdown code here..

        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const juce::String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }



private:
    std::unique_ptr<MainWindow> mainWindow;
};


// Global Application Properties Helper Functions
static VSTSimpleHostApplication& getApp()         { return *dynamic_cast<VSTSimpleHostApplication*>(JUCEApplication::getInstance()); }
ApplicationProperties& getAppProperties()         { return *getApp().appProperties; }

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (VSTSimpleHostApplication)
