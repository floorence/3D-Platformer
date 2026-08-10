#include"SettingsMenu.h"
#include "controller/Settings.h"
#include "gui/IntGuiElement.h"
#include <memory>

SettingsMenu::SettingsMenu(SettingsController* sc)
    : sc(sc) 
{
    initializeUI(Settings {});
}

void SettingsMenu::initializeUI(const Settings& settings) {
    initPersistentUI();

    for (auto& category: settings.children) {
        Button categoryButton;
        std::string categoryName = category->name;
        categoryButton.setText(categoryName);
        categoryButton.setBackgroundColor(GREY_4);
        categoryButton.setOnClick([this, categoryName]() {
            currentTab = categoryName;
        });
        categoryButtons.push_back(std::move(categoryButton));

        int i = 0;
        for (auto& setting: category->getChildren()) {
            initGuiElementsFor(*setting, i);
            i++;
        }
    }
}

void SettingsMenu::initPersistentUI() {
    background.useColorInsteadOfTexture = true;
    background.color = GREY_3;

    headerBackground.useColorInsteadOfTexture = true;
    headerBackground.color = GREY_5;

    footerBackground.useColorInsteadOfTexture = true;
    footerBackground.color = GREY_5;

    closeButton.setText("x");
    closeButton.setBackgroundColor(GREY_7);
    closeButton.setOnClick([this]() {
        isOpen = false;
    });

    applyButton.setText("Apply");
    applyButton.setBackgroundColor(GREY_7);
    applyButton.setOnClick([this]() {
        this->sc->save(readSettings());
    });

    cancelButton.setText("Cancel");
    cancelButton.setBackgroundColor(GREY_7);
    cancelButton.setOnClick([this]() {
        isOpen = false;
    });
}

void SettingsMenu::initGuiElementsFor(Setting& setting, int index) {
    SettingGui settingGui;
    
    settingGui.index = index;
    settingGui.description = std::make_unique<Text>(Text(setting.name));

    switch (setting.guiRepresentation) {
        case SettingGuiElement::Stepper: {
            Stepper stepper;
            stepper.setColors(GREY_5, textColor);
            stepper.setMinMax(setting.minValue, setting.maxValue);
            stepper.setStepAmount((setting.maxValue - setting.minValue > 50) ? 10 : 1); // TODO
            settingGui.guiElement = std::make_unique<Stepper>(std::move(stepper));
            break;
        }
        case SettingGuiElement::Toggle: {
            Toggle toggle;
            toggle.setColors(GREY_4, GREY_5);
            settingGui.guiElement = std::make_unique<Toggle>(std::move(toggle));
            break;
        }
        case SettingGuiElement::CycleButton: {
            // TODO
            break;
        }
    }

    settingGuiData.push_back(std::move(settingGui));
}

void SettingsMenu::onBoundsChanged() {
    float headerFooterHeight = h / 10;
    float tabHeight = h / 12;
    float settingHeight = h / 20;
    float sideBarWidth = w / 4;
    float buttonsHeight = headerFooterHeight * 2/3;
    float padding = headerFooterHeight / 6;

    float endWithPadding = x + w - padding;
    float bottomWithPadding = y + h - padding;
    float setting1Y = y + headerFooterHeight + padding;

    background.setBounds(x, y, w, h);
    headerBackground.setBounds(x, y, w, headerFooterHeight);
    footerBackground.setBounds(x, y + h - headerFooterHeight, w, headerFooterHeight);

    settingsTitle.setFontSize(headerFooterHeight * 2/3);
    settingsTitle.center(x, x + w, y, y + headerFooterHeight);

    closeButton.setBounds(endWithPadding - buttonsHeight, y + padding, buttonsHeight, buttonsHeight);
    applyButton.setBounds(endWithPadding - buttonsHeight * 2, bottomWithPadding - buttonsHeight, buttonsHeight * 2, buttonsHeight);
    cancelButton.setBounds(endWithPadding - buttonsHeight * 4 - padding, bottomWithPadding - buttonsHeight, buttonsHeight * 2, buttonsHeight);

    for (uint i = 0; i < categoryButtons.size(); i++) {
        categoryButtons[i].setBounds(x, y + headerFooterHeight + tabHeight * i, sideBarWidth, tabHeight);
    }

    for (uint i = 0; i < settingGuiData.size(); i++) {
        float settingY = setting1Y + (settingHeight + padding) * settingGuiData[i].index;

        settingGuiData[i].description->setFontSize(settingHeight * 2/3);
        settingGuiData[i].description->centerVertically(
            x + sideBarWidth + padding, 
            settingY, 
            settingY + settingHeight
        );

        settingGuiData[i].guiElement->setBounds(
            endWithPadding - settingHeight * 4, // TODO
            settingY,
            settingHeight * 4, // TODO
            settingHeight
        );
    }
}

bool SettingsMenu::dispatchMouseEvent(float x, float y, MouseEvent event) {
    if (!isOpen) return false;
    std::vector<Clickable*> clickables = {&closeButton, &cancelButton, &applyButton};
    for (auto& button: categoryButtons) {
        clickables.push_back(&button);
    }

    /*
    if (currentTab == "Graphics") {
        clickables.insert(clickables.end(), );
    } else if (currentTab == "Controls") {
        clickables.insert(clickables.end(), {&sensitivityStepper});
    }
    */

    for (auto& clickable: clickables) {
        bool registeredClick = clickable->dispatchMouseEvent(x, y, event);
        if (event != MouseEvent::Hover && registeredClick) return true;
    }
    return false;
}

void SettingsMenu::onSettingsChanged(const Settings& settings) {
    /*
    bloomStepper.setData(settings.graphics.bloomAmount);
    vsyncToggle.setData(settings.graphics.vsync);
    sensitivityStepper.setData(settings.controls.sensitivity);
    */
    initializeUI(settings);
}

Settings SettingsMenu::readSettings() {
    Settings settings;
    /*
    settings.graphics.bloomAmount = bloomStepper.getData();
    settings.graphics.vsync = vsyncToggle.getData();
    settings.controls.sensitivity = sensitivityStepper.getData();
    */
    return settings;
}

void SettingsMenu::draw() {
    background.draw();
    headerBackground.draw();
    footerBackground.draw();
    settingsTitle.draw();
    closeButton.draw();
    applyButton.draw();
    cancelButton.draw();

    for (auto& button: categoryButtons) {
        button.draw();
    }

    /*
    switch (currentTab) {
        case SettingsCategory::Graphics:
            bloomDesc.draw();
            bloomStepper.draw();
            vsyncDesc.draw();
            vsyncToggle.draw();
            break;
        case SettingsCategory::Controls:
            sensitivityDesc.draw();
            sensitivityStepper.draw();
            break;
    }
    */
}