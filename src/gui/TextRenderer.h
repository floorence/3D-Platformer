#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "texture/FontTexture.h"
#include"shader/Shader.h"

class TextRenderer {
public:
    TextRenderer(int windowWidth, int windowHeight);
    void drawText(const std::string& text, int x, int y, int w, int lineHeight, glm::vec3 textColor = glm::vec3(1.0f, 1.0f, 1.0f));
private:
    int windowWidth, windowHeight;
    FontTexture fontTex;
    Shader fontShader;
    const std::string TAG = "TextRenderer";
};

#endif