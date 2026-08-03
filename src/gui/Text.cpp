#include"Text.h"
#include "texture/FontTexture.h"
#include"util/Globals.h"
#include "util/Log.h"
#include <fmt/format.h>

Text::Text() {
    mesh.setTextures({Globals::Font});
}

Text::Text(std::string text): Text() {
    setText(text);
}

void Text::setText(std::string text) {
    this->text = text;
    onBoundsChanged();
}

void Text::setFontSize(int fontSize) {
    this->fontSize = fontSize;
    onBoundsChanged();
}

void Text::setTextAndSize(std::string text, int fontSize) {
    this->text = text;
    this->fontSize = fontSize;
    onBoundsChanged();
}

void Text::setCenterText(bool centerText) {
    this->centerText = centerText;
    onBoundsChanged();
}

void Text::setPosition(float x, float y) {
    std::pair<float, float> textSize = Globals::Font->getSize(text, fontSize);
    setBounds(x, y, textSize.first, textSize.second);
}

void Text::center(float startX, float endX, float startY, float endY) {
    if (centerText) {
        Log::warn("Text", "center() called when centerText is true which is redundant!");
    }
    std::pair<float, float> textSize = Globals::Font->getSize(text, fontSize);
    Rect::center(textSize.first, textSize.second, startX, endX, startY, endY);
}

void Text::onBoundsChanged() {
    float textX = x;
    float textY = y;

    if (centerText) {
        std::pair<float, float> textSize = Globals::Font->getSize(text, fontSize);
        //Log::log("Text", fmt::format("onBoundsChanged() text: {} width: {} height: {}", text, textSize.first, textSize.second));
        if (textSize.first < w) {
            textX = x + w / 2 - textSize.first / 2;
        }
        if (textSize.second < h) {
            textY = y + h / 2 - textSize.second / 2;
        }
    }
    //Log::log("Text", fmt::format("x: {}, y: {}, w: {}, h: {}, textX: {}, textY: {}", x, y, w, h, textX, textY));

    std::vector<Vertex> vertices = Globals::Font->generateVertices(text, textX, textY, fontSize, w);
    std::vector<GLuint> indices = Globals::Font->generateIndices(vertices);

    mesh.setShapeData(vertices, indices);
}

void Text::draw(Shader& shader) {
    shader.setTextColor(textColor);
    mesh.drawGui(shader);
}