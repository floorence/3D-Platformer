#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <vector>

/*
  Places to change to add a new setting
  1. this file: add setting to corresponding struct with default value, min/max values if applicable, and gui representation.
     don't forget to register setting in getChildren() of corresponding struct as well
  2. save/settings.json: add initial value of setting otherwise first load will cause crash
  3. in the class that your setting affects, extend SettingsListener and register it in main if not already, 
     and implement onSettingsChanged
*/

enum class SettingGuiElement {
    Toggle, Stepper, CycleButton
};

struct Setting {
    std::string name;
    int value;
    SettingGuiElement guiRepresentation;
    int minValue = 0, maxValue = 0; // for stepper
    std::vector<std::string> options = {}; // for cycleButton
};

struct SettingsCategory {
    std::string name;
    SettingsCategory(std::string name): name(name) {};

    virtual std::vector<Setting*> getChildren() = 0;
};

struct GraphicsSettings: SettingsCategory {
    GraphicsSettings(): SettingsCategory("Graphics") {};
    Setting bloomAmount {"Bloom Amount", 1, SettingGuiElement::Stepper, 0, 4}; // blurAmount = bloomAmount * 10
    Setting shadowQuality {"Shadow Quality", 2, SettingGuiElement::CycleButton, 0, 2, 
        {"Off", "Low", "High"}
    };
    Setting vsync {"Vsync", true, SettingGuiElement::Toggle};

    std::vector<Setting*> getChildren() override {
        return {&bloomAmount, &shadowQuality, &vsync};
    }
};

struct ControlsSettings: SettingsCategory {
    ControlsSettings(): SettingsCategory("Controls") {};
    Setting sensitivity {"Sensitivity", 100, SettingGuiElement::Stepper, 10, 200};

    std::vector<Setting*> getChildren() override {
        return {&sensitivity};
    }
};

struct Settings {
    GraphicsSettings graphics;
    ControlsSettings controls;

    std::vector<SettingsCategory*> children = {&graphics, &controls};
};

#endif