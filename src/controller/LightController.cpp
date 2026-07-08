#include "LightController.h"
#include "camera/PointLightCamera.h"
#include "texture/CubeMapTexture.h"
#include "util/Log.h"

LightController::LightController() 
    : depthMapTexture(DEPTH_MAP_WIDTH, DEPTH_MAP_HEIGHT),
      depthShader("shader/depth.vert", "shader/depth.geom", "shader/depth.frag"),
      pointLightCam(glm::vec3(0.0f), DEPTH_MAP_WIDTH, DEPTH_MAP_HEIGHT)
{
    initDepthMap();
}

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

void LightController::processLighting(Shader& shader) {
    int numPointLights = 0;

    for (const auto& light: lights) {
        float linear, quadratic;
        calculateAttenuationCoefficients(light->intensity, &linear, &quadratic);
        shader.registerLightSource(
            numPointLights,
            light->color,
            light->position,
            linear, quadratic
        );
        numPointLights++;
    }

    shader.setNumPointLights(numPointLights);
}

void LightController::processShadows(Shader& shader) {
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFboID);
    glViewport(0, 0, DEPTH_MAP_WIDTH, DEPTH_MAP_HEIGHT);
    glClear(GL_DEPTH_BUFFER_BIT);
//    glCullFace(GL_FRONT);

    pointLightCam.position = lights[0]->position;
    pointLightCam.generateTransforms();
    for (const auto& shape : shapes) {
        shape->drawToDepthMap(pointLightCam, depthShader);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
 //   glCullFace(GL_BACK);

    shader.setCubeMapTexture(depthMapTexture, "depthMap", 5);
    shader.setFarPlane(pointLightCam.farPlane);
}

void LightController::initDepthMap() {
    glGenFramebuffers(1, &depthMapFboID);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFboID);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMapTexture.ID, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Log::err(TAG, "Depth framebuffer incomplete");
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    pointLightCam.setPerspective(90.0f, 0.1f, 10.0f);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightController::calculateAttenuationCoefficients(float range, float* linear, float* quadratic) {
    *linear = LINEAR_COEFFICIENT * pow(range, LINEAR_POWER);
    *quadratic = QUADRATIC_COEFFICIENT * pow(range, QUADRATIC_POWER);
}

