#ifndef SETTINGS_H
#define SETTINGS_H

/*
  Places to change to add a new setting
  1. this file: add setting to corresponding struct
  2. save/settings.json: add initial value of setting otherwise first load will cause crash
  3. SettingsController.cpp: handling new setting in save and load functions
  4. SettingsMenu.h: add gui elements for your setting
  4. SettingsMenu.cpp: intialize the gui elements, size them accordingly in onBoundsChanged, handle them in
     dispatchMouseEvent, readSettings, and draw
  5. in the class that your setting affects, extend SettingsListener and register it in main if not already, 
     and implement onSettingsChanged
  TODO: wow that's a lot, maybe i should make a settings ui generator based off the structs
*/

struct GraphicsSettings {
    int bloomAmount = 1; // blurAmount = bloomAmount * 10
    bool vsync = true;
    bool fullscreen = false; // TODO
};

struct ControlsSettings {
    int sensitivity = 100;
};

struct Settings {
    GraphicsSettings graphics;
    ControlsSettings controls;
};

#endif