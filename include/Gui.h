#ifndef GUI_CLASS_H
#define GUI_CLASS_H

#include "FontTexture.h"
#include "ImageTexture.h"
#include "Mesh.h"

class Gui {
public:
    Gui(FontTexture* t, int windowW, int windowH);
    Gui(ImageTexture* t, int windowW, int windowH);

    void draw(int x, int y, int w, int h);
    void drawText(const std::string& text, int x, int y, int w, int lineHeight); // texture in material has to be a FontTexture, otherwise weird things will happen probably
private:
    FontTexture* fontTex = nullptr;
    ImageTexture* imageTex = nullptr;
    Shader shader;
    const std::string TAG = "Gui";

    void configureShader(int windowW, int windowH);
};

#endif