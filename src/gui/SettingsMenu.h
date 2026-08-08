#ifndef SETTINGSMENU_H
#define SETTINGSMENU_H

#include "controller/SettingsController.h"
#include "controller/SettingsListener.h"
#include "gui/Quad.h"
#include "gui/Stepper.h"
#include "gui/Toggle.h"

enum class SettingsTab {
    Graphics, Controls
};

class SettingsMenu: public Rect, public SettingsListener, public Clickable {
public:
    bool isOpen = false;

    SettingsMenu(SettingsController* sc);

    void draw(Shader& shader, Shader& fontShader);
    bool dispatchMouseEvent(float x, float y, MouseEvent event) override;
    void onSettingsChanged(const Settings& settings) override;
private:
    Text settingsTitle = Text("Settings");
    Button graphicsTab;
    Button controlsTab;
    glm::vec3 textColor = glm::vec3(1.0f, 1.0f, 1.0f);

    Quad background;
    Quad headerBackground;
    Quad footerBackground;

    // graphics
    Text bloomDesc = Text("Bloom amount");
    Stepper bloomStepper;
    Text vsyncDesc = Text("Vsync");
    Toggle vsyncToggle;

    // controls
    Text sensitivityDesc = Text("Sensitivity");
    Stepper sensitivityStepper;

    Button closeButton;
    Button applyButton;
    Button cancelButton;

    const glm::vec3 GREY_1 = glm::vec3(0.1f, 0.1f, 0.1f);
    const glm::vec3 GREY_2 = glm::vec3(0.2f, 0.2f, 0.2f);
    const glm::vec3 GREY_3 = glm::vec3(0.3f, 0.3f, 0.3f);
    const glm::vec3 GREY_4 = glm::vec3(0.4f, 0.4f, 0.4f);
    const glm::vec3 GREY_5 = glm::vec3(0.5f, 0.5f, 0.5f);
    const glm::vec3 GREY_6 = glm::vec3(0.6f, 0.6f, 0.6f);
    const glm::vec3 GREY_7 = glm::vec3(0.7f, 0.7f, 0.7f);

    SettingsController* sc;
    SettingsTab currentTab = SettingsTab::Graphics;

    void onBoundsChanged() override;
    Settings readSettings();
};

#endif