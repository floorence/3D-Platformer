#include "Hud.h"
#include "gui/SettingsMenu.h"
#include "util/Globals.h"
#include "util/Log.h"

Hud::Hud(int windowWidth, int windowHeight, SettingsMenu* settingsMenu) {
	settingsButton.setBoundsEnd(windowWidth - 10, windowHeight - 40, 70, 30);
	settingsButton.setText("settings");
	settingsButton.setBackgroundColor(glm::vec3(1.0f, 0.71f, 0.957f));
	settingsButton.setOnClick([settingsMenu]() {
		Log::log("Hud", "settings button clicked");
		settingsMenu->isOpen = !settingsMenu->isOpen;
	});

	performanceText.setBoundsEnd(windowWidth, 10, 200, 100);
	performanceText.setFontSize(16);
	performanceText.setCenterText(false);
}

bool Hud::dispatchMouseEvent(float x, float y, MouseEvent event) {
    return settingsButton.dispatchMouseEvent(x, y, event);
}

void Hud::onWindowSizeChanged(int newWidth, int newHeight) {
    Log::log("Hud", fmt::format("onWindowSizeChanged({}, {})", newWidth, newHeight));
	glm::mat4 guiProjection = glm::ortho(0.0f, (float)newWidth, (float)newHeight, 0.0f, -1.0f, 1.0f);	
	Globals::GuiShader->setProjection(guiProjection);
	Globals::FontShader->setProjection(guiProjection);

	settingsButton.setBoundsEnd(newWidth - 10, newHeight - 40, 70, 30);
	performanceText.setBoundsEnd(newWidth, 10, 200, 100);
}

void Hud::setPerformanceText(std::string text) {
    performanceText.setText(text);
}

void Hud::draw() {
    settingsButton.draw();
    performanceText.draw();
}