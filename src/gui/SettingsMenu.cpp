#include"SettingsMenu.h"

SettingsMenu::SettingsMenu(float xu, float yu, float xv, float yv) {
    setCorners(xu, yu, xv, yv);

    background.setCorners(xu, yu, xv, yv);
    background.useColorInsteadOfTexture = true;
    background.color = glm::vec3(0.22f, 0.22f, 0.22f);

    bloomStepper.setCorners(xu + padding, yu + padding, xu + padding + 100, yu + padding + 40);
    bloomStepper.setColors(glm::vec3(0.7f, 0.7f, 0.7f), textColor);
    bloomStepper.setCountAndMinMax(1, 0, 4);
    vsyncToggle.setCorners(xu + padding, yu + padding + 40 + padding, xu + padding + 60, yu + padding + 40 + padding + 40);
    vsyncToggle.setColors(glm::vec3(0.47f, 0.47f, 0.47f), glm::vec3(0.7f, 0.7f, 0.7f));
}

std::vector<Clickable*> SettingsMenu::getClickables() {
    return {&bloomStepper, &vsyncToggle};
}

void SettingsMenu::draw(Shader& shader, Shader& fontShader) {
    background.draw(shader);
    bloomStepper.draw(shader, fontShader);
    vsyncToggle.draw(shader);
}