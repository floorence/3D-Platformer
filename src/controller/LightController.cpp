#include "LightController.h"
#include "camera/PointLightCamera.h"
#include "gui/Quad.h"
#include "texture/CubeMapTexture.h"
#include "util/Log.h"

LightController::LightController(int windowWidth, int windowHeight) 
    : windowWidth(windowWidth), windowHeight(windowHeight),
      depthMapTexture(DEPTH_MAP_WIDTH, DEPTH_MAP_HEIGHT),
      depthShader("shader/depth.vert", "shader/depth.geom", "shader/depth.frag"),
      pointLightCam(glm::vec3(0.0f), DEPTH_MAP_WIDTH, DEPTH_MAP_HEIGHT),
      colorBufTexture("hdrBuffer"),
      hdrShader("shader/gui.vert", "shader/hdr.frag")
{
    prepareDepthMap();
    prepareHDR();
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

void LightController::renderForShadows(Shader& shader) {
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFboID);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // THIS NEEDS TO BE AFTER BIND!!!
    glViewport(0, 0, DEPTH_MAP_WIDTH, DEPTH_MAP_HEIGHT);

    pointLightCam.position = lights[0]->position;
    pointLightCam.generateTransforms();
    for (const auto& shape : shapes) {
        shape->drawToDepthMap(pointLightCam, depthShader);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    shader.setCubeMapTexture(depthMapTexture, "depthMap", 5);
    shader.setFarPlane(pointLightCam.farPlane);

    glViewport(0, 0, windowWidth, windowHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void LightController::renderForHDR(Shader& shader, Shader& lightShader, Camera& camera) {
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFboID);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto& shape: shapes) {
        shape->draw(camera, shader);
    }
    for (const auto& light: lights) {
        light->draw(camera, lightShader);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void LightController::renderForReal() {
    hdrShader.setTexture(colorBufTexture, colorBufTexture.getUniformName(), 0);
	hdrShader.setProjection(glm::mat4(1.0f));
    Quad hdr(&colorBufTexture);
    hdr.draw(hdrShader, CoordinateSystem2D::CENTER, -1.0, 1.0, 2.0, 2.0);
}
    
void LightController::prepareDepthMap() {
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

void LightController::prepareHDR() {
    // configure floating point framebuffer
    glGenFramebuffers(1, &hdrFboID);
    // create floating point color buffer

    colorBufTexture.bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // create depth buffer (renderbuffer)
    // THIS IS NEEDED TO RESOLVE VERTICES!!! (texture only does colours)
    GLuint rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowWidth, windowHeight);
    // attach buffers
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFboID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBufTexture.ID, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Log::err(TAG, "Framebuffer incomplete");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightController::calculateAttenuationCoefficients(float range, float* linear, float* quadratic) {
    *linear = LINEAR_COEFFICIENT * pow(range, LINEAR_POWER);
    *quadratic = QUADRATIC_COEFFICIENT * pow(range, QUADRATIC_POWER);
}

