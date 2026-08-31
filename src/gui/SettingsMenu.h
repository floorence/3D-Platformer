#ifndef SETTINGSMENU_H
#define SETTINGSMENU_H

#include "WindowListener.h"
#include "controller/SettingsController.h"
#include "controller/SettingsListener.h"
#include "gui/Button.h"
#include "gui/IntGuiElement.h"
#include "gui/Quad.h"
#include <memory>
#include "Text.h"

struct SettingGui {
    std::unique_ptr<Text> description; // could store just Text. but unique_ptrs are nice and it would be consistent with guiElement.
    std::unique_ptr<IntGuiElement> guiElement; // pointer because IntGuiElement is abstract
};

class SettingsMenu: public Rect, public SettingsListener, public Clickable, public WindowListener {
public:
    bool isOpen = false;

    SettingsMenu(SettingsController* sc);

    void draw();
    bool dispatchMouseEvent(float x, float y, MouseEvent event) override;
    void onSettingsChanged(const Settings& settings) override;
    void onWindowSizeChanged(int newWidth, int newHeight) override;
private:
    Text settingsTitle = Text("Settings");
    Quad background;
    Quad headerBackground;
    Quad footerBackground;

    Button closeButton;
    Button applyButton;
    Button cancelButton;

    glm::vec3 textColor = glm::vec3(1.0f, 1.0f, 1.0f);

    std::vector<Button> categoryButtons;
    std::vector<std::vector<SettingGui>> settingGuiData;

    const glm::vec3 GREY_1 = glm::vec3(0.1f, 0.1f, 0.1f);
    const glm::vec3 GREY_2 = glm::vec3(0.2f, 0.2f, 0.2f);
    const glm::vec3 GREY_3 = glm::vec3(0.3f, 0.3f, 0.3f);
    const glm::vec3 GREY_4 = glm::vec3(0.4f, 0.4f, 0.4f);
    const glm::vec3 GREY_5 = glm::vec3(0.5f, 0.5f, 0.5f);
    const glm::vec3 GREY_6 = glm::vec3(0.6f, 0.6f, 0.6f);
    const glm::vec3 GREY_7 = glm::vec3(0.7f, 0.7f, 0.7f);
    const std::string TAG = "SettingsMenu";

    SettingsController* sc;
    int currentTab = 0;

    void initialize(const Settings& settings);
    void initPersistentUI();
    void initGuiElementsFor(Setting& setting, int categoryNum);

    void onBoundsChanged() override;
    Settings readSettings();
};

#endif