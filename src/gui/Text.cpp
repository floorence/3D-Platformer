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

void Text::onBoundsChanged() {
    std::pair<float, float> textSize = Globals::Font->getSize(text, fontSize);
    //Log::log("Text", fmt::format("onBoundsChanged() text: {} width: {} height: {}", text, textSize.first, textSize.second));
    // center text within bounds
    float textX = x;
    float textY = y;
    if (centerText && textSize.first < w) {
        textX = x + w / 2 - textSize.first / 2;
    }
    if (centerText && textSize.second < h) {
        textY = y + h / 2 - textSize.second / 2;
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