#ifndef BUTTON_H
#define BUTTON_H

#include "gui/Clickable.h"
#include "gui/Quad.h"
#include "gui/TextRenderer.h"
#include <functional>

class Button: public Quad, public Clickable {
public:
    std::string text = "";
    float padding = 2.0f; // TODO

    Button() = default;
    Button(float xu, float yu, float xv, float yv);

    void draw(Shader& shader, TextRenderer* textRenderer);
    void setOnClick(std::function<void()> callback);
    bool dispatchClick(float x, float y) override;
private:
    std::function<void()> onClickCallback;
};

#endif