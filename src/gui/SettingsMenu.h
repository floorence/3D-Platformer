#ifndef SETTINGSMENU_H
#define SETTINGSMENU_H

#include "controller/SettingsController.h"
#include "gui/Quad.h"
#include "gui/Stepper.h"
#include "gui/Toggle.h"


class SettingsMenu: public Rect {
public:
    bool isOpen = false;

    SettingsMenu(SettingsController* sc);

    std::vector<Clickable*> getClickables();
    void draw(Shader& shader, Shader& fontShader);
private:
    Text settingsTitle = Text("Settings");
    Text graphicsSubtitle = Text("Graphics");
    glm::vec3 textColor = glm::vec3(1.0f, 1.0f, 1.0f);
    float padding = 5.0f;

    Quad background;
    Text bloomDesc = Text("Bloom amount");
    Stepper bloomStepper;
    Text vsyncDesc = Text("Vsync");
    Toggle vsyncToggle;

    Button saveButton;
    Button cancelButton;

    const glm::vec3 DARK_GREY = glm::vec3(0.22f, 0.22f, 0.22f);
    const glm::vec3 MEDIUM_GREY = glm::vec3(0.47f, 0.47f, 0.47f);
    const glm::vec3 LIGHT_GREY = glm::vec3(0.7f, 0.7f, 0.7f);

    SettingsController* sc;

    void onBoundsChanged() override;
    Settings readSettings();
};

#endif