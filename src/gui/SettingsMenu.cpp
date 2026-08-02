#include"SettingsMenu.h"

SettingsMenu::SettingsMenu(SettingsController* sc)
    : sc(sc) 
{
    background.useColorInsteadOfTexture = true;
    background.color = DARK_GREY;

    settingsTitle.setCenterText(false);
    settingsTitle.setFontSize(40);

    bloomDesc.setFontSize(20);
    bloomStepper.setColors(LIGHT_GREY, textColor);
    bloomStepper.setCountAndMinMax(1, 0, 4);

    vsyncDesc.setFontSize(20);
    vsyncToggle.setColors(MEDIUM_GREY, LIGHT_GREY);

    saveButton.setText("Save");
    saveButton.setBackgroundColor(LIGHT_GREY);
    saveButton.setOnClick([this]() {
        this->sc->save(readSettings());
        isOpen = false;
    });

    cancelButton.setText("Cancel");
    cancelButton.setBackgroundColor(LIGHT_GREY);
    cancelButton.setOnClick([this]() {
        isOpen = false;
    });
}

void SettingsMenu::onBoundsChanged() {
    // TODO: not everything here has to be redone every time bounds changes
    background.setBounds(x, y, w, h);

    settingsTitle.center(x, x + w, y, y + 50);

    bloomDesc.setPosition(x + padding, y + 50 + padding);
    bloomStepper.setBounds(x + w - padding - 120, y + 50 + padding, 120, 40);

    vsyncDesc.setPosition(x + padding, y + 90 + padding * 2);
    vsyncToggle.setBounds(x + w - padding - 80, y + 90 + padding * 2, 80, 40);

    saveButton.setBounds(x + w - padding - 80, y + h - padding - 40, 80, 40);

    cancelButton.setBounds(x + w - 160 - padding * 2, y + h - padding - 40, 80, 40);
}

Settings SettingsMenu::readSettings() {
    Settings settings;
    settings.graphics.bloomAmount = bloomStepper.getCount();
    settings.graphics.vsync = vsyncToggle.getIsOn();
    return settings;
}

std::vector<Clickable*> SettingsMenu::getClickables() {
    return {&bloomStepper, &vsyncToggle, &saveButton, &cancelButton};
}

void SettingsMenu::draw(Shader& shader, Shader& fontShader) {
    background.draw(shader);
    settingsTitle.draw(fontShader);
    bloomDesc.draw(fontShader);
    bloomStepper.draw(shader, fontShader);
    vsyncDesc.draw(fontShader);
    vsyncToggle.draw(shader);
    saveButton.draw(shader, fontShader);
    cancelButton.draw(shader, fontShader);
}