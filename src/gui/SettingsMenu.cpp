#include"SettingsMenu.h"

SettingsMenu::SettingsMenu(float xu, float yu, float xv, float yv) {
    setCorners(xu, yu, xv, yv);
}

void SettingsMenu::onBoundsChanged() {
    background.setBounds(x, y, w, h);
    background.useColorInsteadOfTexture = true;
    background.color = glm::vec3(0.22f, 0.22f, 0.22f);

    settingsTitle.setCenterText(false);
    settingsTitle.setFontSize(40);
    settingsTitle.center(x, x + w, y, y + 50);

    bloomDesc.setFontSize(20);
    bloomDesc.setPosition(x + padding, y + 50 + padding);
    bloomStepper.setBounds(x + w - padding - 120, y + 50 + padding, 120, 40);
    bloomStepper.setColors(glm::vec3(0.7f, 0.7f, 0.7f), textColor);
    bloomStepper.setCountAndMinMax(1, 0, 4);

    vsyncDesc.setFontSize(20);
    vsyncDesc.setPosition(x + padding, y + 90 + padding * 2);
    vsyncToggle.setBounds(x + w - padding - 80, y + 90 + padding * 2, 80, 40);
    vsyncToggle.setColors(glm::vec3(0.47f, 0.47f, 0.47f), glm::vec3(0.7f, 0.7f, 0.7f));
}

std::vector<Clickable*> SettingsMenu::getClickables() {
    return {&bloomStepper, &vsyncToggle};
}

void SettingsMenu::draw(Shader& shader, Shader& fontShader) {
    background.draw(shader);
    settingsTitle.draw(fontShader);
    bloomDesc.draw(fontShader);
    bloomStepper.draw(shader, fontShader);
    vsyncDesc.draw(fontShader);
    vsyncToggle.draw(shader);
}