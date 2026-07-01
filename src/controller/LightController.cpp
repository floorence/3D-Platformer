#include "LightController.h"

void LightController::registerShape(Shape3D* shape) {
    if (shape->isLightSource)
        lights.push_back(shape);
    else
        shapes.push_back(shape);
}

void LightController::registerShapes(std::vector<Shape3D*> shapes) {
    for (const auto& shape :shapes) {
        registerShape(shape);
    }
}

void LightController::processLighting() {
    int* numPointLights = new int[shapes.size()]{};

    for (const auto& light: lights) {
        float linear, quadratic;
        calculateAttenuationCoefficients(light->light.range, &linear, &quadratic);

        for (uint i = 0; i < shapes.size(); i++) {
            float distance = glm::length(light->position - shapes[i]->position);
            if (distance < light->light.range) {
                shapes[i]->registerLightSource(
                    numPointLights[i],
                    light->light.color,
                    light->position,
                    linear, quadratic
                );
                numPointLights[i]++;
            }
        }
    }

    for (uint i = 0; i < shapes.size(); i++) {
        shapes[i]->setNumPointLights(numPointLights[i]);
    }

    delete[] numPointLights;
}

void LightController::calculateAttenuationCoefficients(float range, float* linear, float* quadratic) {
    //TODO
    *linear = 0.35;
    *quadratic = 0.44;
}
