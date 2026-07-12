#ifndef GUI_CLASS_H
#define GUI_CLASS_H

#include"Quad.h"
#include "texture/ImageTexture.h"

// TODO
class Gui: public Quad {
public:
    Gui(ImageTexture* t);
private:
    ImageTexture* imageTex = nullptr;
    const std::string TAG = "Gui";
};

#endif