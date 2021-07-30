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

    return menu;
}

void MainWindow::menuItemSelected (int menuItemID, int topLevelMenuIndex)
{
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

void MainWindow::updateDeviceSettings()
{
    //deviceManager.
    //mainComponent->updateDeviceSettings(this->deviceManager.);
}
