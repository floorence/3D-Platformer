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
    int minValue, maxValue;
    SettingGuiElement guiRepresentation;
};

struct SettingsCategory {
    std::string name;
    SettingsCategory(std::string name): name(name) {};

    virtual std::vector<Setting*> getChildren() = 0;
};

struct GraphicsSettings: SettingsCategory {
    GraphicsSettings(): SettingsCategory("Graphics") {};
    Setting bloomAmount {"Bloom Amount", 1, 0, 4, SettingGuiElement::Stepper}; // blurAmount = bloomAmount * 10
    Setting vsync {"Vsync", true, 0, 1, SettingGuiElement::Toggle};

    std::vector<Setting*> getChildren() override {
        return {&bloomAmount, &vsync};
    }
};

struct ControlsSettings: SettingsCategory {
    ControlsSettings(): SettingsCategory("Controls") {};
    Setting sensitivity {"Sensitivity", 100, 10, 200, SettingGuiElement::Stepper};

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