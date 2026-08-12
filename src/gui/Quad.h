#ifndef QUAD_H
#define QUAD_H

#include "gui/Rect.h"
#include "mesh/Mesh.h"
#include"texture/Texture.h"
#include"shader/Shader.h"
#include "util/Globals.h"

class Quad: public Rect {
public:
    bool useColorInsteadOfTexture = false;
    glm::vec3 color;
    glm::vec4 tintColor = glm::vec4(0.0f);
    // true if y increases downward. it's true by default since that's how the gui shader is set up. 
    // it's only false for Quads that are drawn during lighting processing. see LightController.cpp
    // TODO literally not true ur stupid
    bool reversedYAxis = true;

    Quad() = default;
    Quad(Texture* texture);
    Quad(float x, float y, float w, float h, bool corners = false);

    void draw(Shader& shader = *Globals::GuiShader);

    // Textures; if none is set and useColorInsteadOfTexture = true then the quad will be transparent

    void setTexture(Texture* texture);
    void setTextures(std::vector<Texture*> textures);
private:
    Mesh mesh;

    void onBoundsChanged() override;
};

#endif