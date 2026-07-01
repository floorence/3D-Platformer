#ifndef LIGHT_CONTROLLER_H
#define LIGHT_CONTROLLER_H

#include "Shape3D.h"

class LightController {
public:
    LightController() = default;

    void registerShape(Shape3D* shape);
    void registerShapes(std::vector<Shape3D*> shapes);
    void processLighting();
private:
    std::vector<Shape3D*> lights;
    std::vector<Shape3D*> shapes;

    void calculateAttenuationCoefficients(float range, float* linear, float* quadratic);
};

#endif