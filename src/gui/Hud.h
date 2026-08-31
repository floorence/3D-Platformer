#pragma once

#include "window/WindowListener.h"
#include "SettingsMenu.h"

class Hud: public Clickable, public WindowListener {
public:
    Hud(int windowWidth, int windowHeight, SettingsMenu* settingsMenu);

    bool dispatchMouseEvent(float x, float y, MouseEvent event) override;
    void onWindowSizeChanged(int newWidth, int newHeight) override;

    void setPerformanceText(std::string text);
    void draw();
private:
	Button settingsButton;
	Text performanceText;
};