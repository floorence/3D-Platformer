#ifndef SETTINGSMENU_H
#define SETTINGSMENU_H

#include "gui/Quad.h"
#include "gui/Stepper.h"
#include "gui/Toggle.h"

class SettingsMenu: public Rect {
public:
    bool isOpen = false;

    SettingsMenu(float xu, float yu, float xv, float yv);

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
};

#endif