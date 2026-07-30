#ifndef SETTINGSMENU_H
#define SETTINGSMENU_H

#include "gui/Quad.h"
#include "gui/Stepper.h"
#include "gui/TextRenderer.h"
#include "gui/Toggle.h"

class SettingsMenu: public Rect {
public:
    bool isOpen = false;

    SettingsMenu(float xu, float yu, float xv, float yv);

    std::vector<Clickable*> getClickables();
    void draw(Shader& shader, Shader& fontShader);
private:
    std::string settingsTitle = "Settings"; // TODO: this never changes, make a text gui element
    std::string graphicsSubtitle = "Graphics";
    glm::vec3 textColor = glm::vec3(1.0f, 1.0f, 1.0f);
    float padding = 5.0f;

    Quad background;
    std::string bloomDesc = "Bloom amount";
    Stepper bloomStepper;
    std::string vsyncDesc = "Vsync";
    Toggle vsyncToggle;
};

#endif