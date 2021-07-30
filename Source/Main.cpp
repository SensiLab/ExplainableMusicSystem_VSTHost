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
        
        mainWindow->setUsingNativeTitleBar (true); // Done in mainWindow
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

    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainComponent class.
    
    class MainWindow    : public juce::DocumentWindow,
                          public MenuBarModel
    {
    public:
        MainWindow (juce::String name)
            : DocumentWindow (name,
                              juce::Desktop::getInstance().getDefaultLookAndFeel()
                                                          .findColour (juce::ResizableWindow::backgroundColourId),
                              DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
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

        void closeButtonPressed() override
        {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

        
        void menuBarActivated (bool isActive) override{
            
        }

        StringArray getMenuBarNames() override{
            // MenuBar Items
            
            StringArray names;
            //names.add ("File");
            //names.add ("Plugins");
            //names.add ("Options");
            //names.add ("Windows");
            names.add ("Experiment");
            return names;
        }
        
        PopupMenu getMenuForIndex (int topLevelMenuIndex, const String& menuName) override{
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

            return menu;
        }
        
        void menuItemSelected (int menuItemID, int topLevelMenuIndex) override{
            // Handle Menu Selection
            if (menuItemID == 200)
            {
                // Performance 1: Update plugins
                experimentMode = ExperimentModes::Practice;
            }else if (menuItemID == 201)
            {
                // Performance 3: Update plugins
                experimentMode = ExperimentModes::FirstPerformance;
            }else if (menuItemID == 202)
            {
                // Performance 2: Update plugins
                experimentMode = ExperimentModes::SecondPerformance;
            }else if (menuItemID == 203)
            {
                // Performance 3: Update plugins
                experimentMode = ExperimentModes::ThirdPerformance;
            }
            
            // Update MainComponent
            mainComponent->setExperimentMode(experimentMode);
            
            
            // Update Menu Item Selection
            menuItemsChanged();
        }
        
        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        
        

    private:
        int experimentMode = ExperimentModes::Practice;
        MainComponent *mainComponent;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };
     */
private:
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// Global Application Properties Helper Functions
static VSTSimpleHostApplication& getApp()         { return *dynamic_cast<VSTSimpleHostApplication*>(JUCEApplication::getInstance()); }
ApplicationProperties& getAppProperties()         { return *getApp().appProperties; }

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (VSTSimpleHostApplication)
