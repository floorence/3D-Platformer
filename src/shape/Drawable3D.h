#ifndef DRAWABLE_3D_H
#define DRAWABLE_3D_H

#include "camera/Camera.h"
#include "mesh/VBO.h"
#include "shader/Shader.h"

class Drawable3D {
public:
    bool cullFacesBeforeDraw = true;
    Shader* specialShader = nullptr; // optional special shader to use instead of default

    Drawable3D() = default;

    virtual ~Drawable3D() = default;

    virtual void draw(Camera& camera, Shader& shader) = 0;
    virtual void drawToDepthMap(PointLightCamera& camera, Shader& depthShader) = 0;
};

#endif