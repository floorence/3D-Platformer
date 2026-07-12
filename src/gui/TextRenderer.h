#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "texture/FontTexture.h"
#include"shader/Shader.h"

class TextRenderer {
public:
    TextRenderer(FontTexture* t);
    void drawText(const std::string& text, Shader& shader, int x, int y, int w, int lineHeight);
private:
    FontTexture* fontTex = nullptr;
    const std::string TAG = "TextRenderer";
};

#endif