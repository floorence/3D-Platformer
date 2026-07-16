#include "LightController.h"
#include "camera/PointLightCamera.h"
#include "texture/CubeMapTexture.h"
#include "util/Log.h"
#include "util/Utils.h"
#include <cmath>

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
    prepareAvgColorBuffer();
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
    depthMapFbo.bindAndClear();
    glViewport(0, 0, DEPTH_MAP_WIDTH, DEPTH_MAP_HEIGHT);

    pointLightCam.position = lights[0]->position;
    pointLightCam.generateTransforms();
    for (const auto& shape : shapes) {
        shape->drawToDepthMap(pointLightCam, depthShader);
    }

    depthMapFbo.unbindAndClear();

    shader.setCubeMapTexture(depthMapTexture, "depthMap", 5);
    shader.setFarPlane(pointLightCam.farPlane);

    glViewport(0, 0, windowWidth, windowHeight);
}

void LightController::renderForHDR(Shader& shader, Shader& lightShader, Camera& camera) {
    hdrFbo.bindAndClear();

    for (const auto& shape: shapes) {
        shape->draw(camera, shader);
    }
    for (const auto& light: lights) {
        light->draw(camera, lightShader);
    }

    hdrFbo.unbindAndClear();
}

void LightController::adjustBrightness(float deltaTime) {
    colorBufTexture.bind();
    glGenerateMipmap(GL_TEXTURE_2D);

    int highestMipLevel = floor(log2(std::max(windowWidth, windowHeight)));

    int nextIndex = pboIndex;
    int currentIndex = (pboIndex + 1) % 2; 
    pboIndex = currentIndex;
    pbos[pboIndex].bind();
    
    // map the buffer pointer directly to CPU memory to read results of the previous frame
    float* src = (float*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
    if (src) {
        GLubyte r = src[0];
        GLubyte g = src[1];
        GLubyte b = src[2];
        GLubyte a = src[3];
        
        //Log::log(TAG, fmt::format("average color r: {}, g: {}, b: {}, a: {}", r, g, b, a));
        float averageBrightness = Utils::getBrightness(r, g, b);
        float newExposure = TARGET_BRIGHTNESS / log(averageBrightness);
        //exposure = (newExposure - exposure) * (1.0f - exp(-deltaTime * adaptationSpeed));
        //Log::log(TAG, fmt::format("exposure: {}", exposure));

        hdrShader.setExposure(newExposure);

        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
    } else {
        Log::err(TAG, fmt::format("failed to read average color buffer!"));
        Utils::checkOpenGlErrors();
    }

    pbos[nextIndex].bind();

    // trigger the gpu transfer for the current frame
    // when a PBO is bound, the last argument is byte offset inside the PBO, rather than a pointer to cpu memory, it returns immediately.
    glGetTexImage(GL_TEXTURE_2D, highestMipLevel, GL_RGBA, GL_FLOAT, 0);

    pbos[nextIndex].unbind();
}

void LightController::renderForReal() {
    hdrShader.setTexture(colorBufTexture, 0);
	hdrShader.setProjection(glm::mat4(1.0f));
    hdrResult.setTexture(&colorBufTexture);
    hdrResult.draw(hdrShader, -1.0f, 1.0f, 1.0f, -1.0f);
}
    
void LightController::prepareDepthMap() {
    depthMapFbo.bind();
    depthMapFbo.attachTextureCube(depthMapTexture.ID);
    depthMapFbo.checkStatus();
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    pointLightCam.setPerspective(90.0f, 0.1f, 10.0f);

    depthMapFbo.unbind();
}

void LightController::prepareHDR() {
    // create floating point color buffer
    colorBufTexture.bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D); // for getting average colour later
    // create depth buffer (renderbuffer)
    // THIS IS NEEDED TO RESOLVE VERTICES!!! (texture only does colours)
    GLuint rboID;
    glGenRenderbuffers(1, &rboID);
    glBindRenderbuffer(GL_RENDERBUFFER, rboID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowWidth, windowHeight);
    // attach buffers
    hdrFbo.bind();
    hdrFbo.attachTexture2D(colorBufTexture.ID);
    hdrFbo.attachRenderBuffer(rboID);
    hdrFbo.checkStatus();
    hdrFbo.unbind();
}

void LightController::prepareAvgColorBuffer() {
    for (int i = 0; i < 2; i++) {
        pbos[i].bind();
        glBufferData(GL_PIXEL_PACK_BUFFER, sizeof(float) * 4, nullptr, GL_STREAM_READ); // 4 float = 1 pixel rgba float
        pbos[i].unbind();
    }
}

void LightController::calculateAttenuationCoefficients(float range, float* linear, float* quadratic) {
    *linear = LINEAR_COEFFICIENT * pow(range, LINEAR_POWER);
    *quadratic = QUADRATIC_COEFFICIENT * pow(range, QUADRATIC_POWER);
}

