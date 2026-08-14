#ifndef QUAD_H
#define QUAD_H

#include "gui/Rect.h"
#include "mesh/Mesh.h"
#include"texture/Texture.h"
#include"shader/Shader.h"
#include "util/Globals.h"

// assumes y increases downward, since that's how the gui shader is set up. 
class Quad: public Rect {
public:
    bool useColorInsteadOfTexture = false;
    glm::vec3 color;
    glm::vec4 tintColor = glm::vec4(0.0f);

    Quad() = default;
    Quad(Texture* texture);
    Quad(float x, float y, float w, float h, bool corners = false);

    void draw(Shader& shader = *Globals::GuiShader);

    // Textures; if none is set and useColorInsteadOfTexture = false then the quad will be transparent

    void setTexture(Texture* texture);
    void setTextures(std::vector<Texture*> textures);
private:
    Mesh mesh;

    void onBoundsChanged() override;
};

#endif