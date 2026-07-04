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

void LightController::processShadows() {
    uint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    uint depthCubemap;
    glGenTextures(1, &depthCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    for (uint i = 0; i < 6; i++) {
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 
            DEPTH_MAP_WIDTH, DEPTH_MAP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL
        );
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Shader depthShader("shader/depth.vert", "shader/depth.geom", "shader/depth.frag");
    Camera lightSourceCam(lights[0]->position, DEPTH_MAP_WIDTH, DEPTH_MAP_HEIGHT);
    lightSourceCam.setPerspective(90.0f, 0.1f, 100.0f);
    std::vector<glm::mat4> shadowTransforms;

    lightSourceCam.lookAt(glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f));
    shadowTransforms.push_back(lightSourceCam.getCameraMatrix());
    lightSourceCam.lookAt(glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f));
    shadowTransforms.push_back(lightSourceCam.getCameraMatrix());
    lightSourceCam.lookAt(glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f));
    shadowTransforms.push_back(lightSourceCam.getCameraMatrix());
    lightSourceCam.lookAt(glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f));
    shadowTransforms.push_back(lightSourceCam.getCameraMatrix());
    lightSourceCam.lookAt(glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f));
    shadowTransforms.push_back(lightSourceCam.getCameraMatrix());
    lightSourceCam.lookAt(glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f));
    shadowTransforms.push_back(lightSourceCam.getCameraMatrix());

    // first render to depth cubemap
    glViewport(0, 0, DEPTH_MAP_WIDTH, DEPTH_MAP_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    // todo render
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // 2. then render scene as normal with shadow mapping (using depth cubemap)
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

void LightController::drawLightsAndShapes() {
    for (const auto& shape : shapes) {
        //shape->draw
    }
}
