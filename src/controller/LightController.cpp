#include "LightController.h"
#include "camera/PointLightCamera.h"
#include "texture/CubeMapTexture.h"

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

void LightController::processShadows() {
    uint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    CubeMapTexture depthCubeMap(DEPTH_MAP_WIDTH, DEPTH_MAP_HEIGHT);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap.ID, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Shader depthShader("shader/depth.vert", "shader/depth.geom", "shader/depth.frag");
    PointLightCamera lightSourceCam(lights[0]->position, DEPTH_MAP_WIDTH, DEPTH_MAP_HEIGHT);
    lightSourceCam.setPerspective(90.0f, 0.1f, 100.0f);
    lightSourceCam.generateTransforms();

    // first render to depth cubemap
    glViewport(0, 0, DEPTH_MAP_WIDTH, DEPTH_MAP_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    for (const auto& shape : shapes) {
        shape->drawToDepthMap(lightSourceCam, depthShader);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // then render scene as normal with shadow mapping (using depth cubemap)
    // glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // ConfigureShaderAndMatrices();
    // glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    // RenderScene();

}

void LightController::calculateAttenuationCoefficients(float range, float* linear, float* quadratic) {
    *linear = LINEAR_COEFFICIENT * pow(range, LINEAR_POWER);
    *quadratic = QUADRATIC_COEFFICIENT * pow(range, QUADRATIC_POWER);
}

