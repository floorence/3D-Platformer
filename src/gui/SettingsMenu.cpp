#include"SettingsMenu.h"

SettingsMenu::SettingsMenu(SettingsController* sc)
    : sc(sc) 
{
    background.useColorInsteadOfTexture = true;
    background.color = GREY_3;

    headerBackground.useColorInsteadOfTexture = true;
    headerBackground.color = GREY_5;

    footerBackground.useColorInsteadOfTexture = true;
    footerBackground.color = GREY_5;

    graphicsTab.setText("Graphics");
    graphicsTab.setBackgroundColor(GREY_4);
    graphicsTab.setOnClick([this]() {
        currentTab = SettingsTab::Graphics;
    });

    controlsTab.setText("Controls");
    controlsTab.setBackgroundColor(GREY_4);
    controlsTab.setOnClick([this]() {
        currentTab = SettingsTab::Controls;
    });

    bloomStepper.setColors(GREY_5, textColor);
    bloomStepper.setMinMax(0, 4);

    vsyncToggle.setColors(GREY_4, GREY_5);

    sensitivityStepper.setColors(GREY_5, textColor);
    sensitivityStepper.setMinMax(10, 200);
    sensitivityStepper.setStepAmount(10);
    sensitivityStepper.setUnits("%");

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
    float setting2Y = setting1Y + settingHeight + padding;
    float setting3Y = setting2Y + settingHeight + padding;

    background.setBounds(x, y, w, h);
    headerBackground.setBounds(x, y, w, headerFooterHeight);
    footerBackground.setBounds(x, y + h - headerFooterHeight, w, headerFooterHeight);

    settingsTitle.setFontSize(headerFooterHeight * 2/3);
    settingsTitle.center(x, x + w, y, y + headerFooterHeight);

    graphicsTab.setBounds(x, y + headerFooterHeight, sideBarWidth, tabHeight);
    controlsTab.setBounds(x, y + headerFooterHeight + tabHeight, sideBarWidth, tabHeight);

    bloomDesc.setFontSize(settingHeight * 2/3);
    bloomDesc.centerVertically(x + sideBarWidth + padding, setting1Y, setting1Y + settingHeight);
    bloomStepper.setBounds(endWithPadding - settingHeight * 3, setting1Y, settingHeight * 3, settingHeight);

    vsyncDesc.setFontSize(settingHeight * 2/3);
    vsyncDesc.centerVertically(x + sideBarWidth + padding, setting2Y, setting2Y + settingHeight);
    vsyncToggle.setBounds(endWithPadding - settingHeight * 2, setting2Y, settingHeight * 2, settingHeight);

    sensitivityDesc.setFontSize(settingHeight * 2/3);
    sensitivityDesc.centerVertically(x + sideBarWidth + padding, setting1Y, setting1Y + settingHeight);
    sensitivityStepper.setBounds(endWithPadding - settingHeight * 4, setting1Y, settingHeight * 4, settingHeight);

    closeButton.setBounds(endWithPadding - buttonsHeight, y + padding, buttonsHeight, buttonsHeight);
    applyButton.setBounds(endWithPadding - buttonsHeight * 2, bottomWithPadding - buttonsHeight, buttonsHeight * 2, buttonsHeight);
    cancelButton.setBounds(endWithPadding - buttonsHeight * 4 - padding, bottomWithPadding - buttonsHeight, buttonsHeight * 2, buttonsHeight);
}

bool SettingsMenu::dispatchMouseEvent(float x, float y, MouseEvent event) {
    if (!isOpen) return false;
    std::vector<Clickable*> clickables = {&closeButton, &cancelButton, &applyButton, &graphicsTab, &controlsTab};

    switch (currentTab) {
        case SettingsTab::Graphics:
            clickables.insert(clickables.end(), {&bloomStepper, &vsyncToggle});
            break;
        case SettingsTab::Controls:
            clickables.insert(clickables.end(), {&sensitivityStepper});
            break;
    }
    for (auto& clickable: clickables) {
        bool registeredClick = clickable->dispatchMouseEvent(x, y, event);
        if (event != MouseEvent::Hover && registeredClick) return true;
    }
    return false;
}

void SettingsMenu::onSettingsChanged(const Settings& settings) {
    bloomStepper.setData(settings.graphics.bloomAmount);
    vsyncToggle.setData(settings.graphics.vsync);
}

Settings SettingsMenu::readSettings() {
    Settings settings;
    settings.graphics.bloomAmount = bloomStepper.getData();
    settings.graphics.vsync = vsyncToggle.getData();
    settings.controls.sensitivity = sensitivityStepper.getData();
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
    graphicsTab.draw();
    controlsTab.draw();

    switch (currentTab) {
        case SettingsTab::Graphics:
            bloomDesc.draw();
            bloomStepper.draw();
            vsyncDesc.draw();
            vsyncToggle.draw();
            break;
        case SettingsTab::Controls:
            sensitivityDesc.draw();
            sensitivityStepper.draw();
            break;
    }
}