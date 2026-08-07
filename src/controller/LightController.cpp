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
      hdrTexture("hdrBuffer"),
      bloomTexture("image"),
      hdrBloomShader("shader/gui.vert", "shader/hdr_bloom.frag"),
      blurTextures{{"image"}, {"image"}},
      blurShader("shader/gui.vert", "shader/blur.frag")
{
    prepareDepthMap();
    prepareHdrAndBloom();
    prepareAvgColorBuffer();
    prepareGaussianBlur();
}

void LightController::registerShape(Shape3D* shape) {
    if (shape->isLightSource)
        lights.push_back(shape);
    else
        shapes.push_back(shape);
}

void LightController::registerShapes(const std::vector<Shape3D*>& shapes) {
    for (const auto& shape :shapes) {
        registerShape(shape);
    }
}

void LightController::processLighting(Shader& shader) {
    int numPointLights = 0;

    for (const auto& light: lights) {
        float linear, quadratic;
        float intensity = Utils::getBrightness(light->color) / 10;
        calculateAttenuationCoefficients(intensity, &linear, &quadratic);
        shader.registerLightSource(
            numPointLights,
            light->color,
            light->getPosition(),
            linear, quadratic
        );
        numPointLights++;
    }

    shader.setNumPointLights(numPointLights);
}

void LightController::renderForShadows(Shader& shader) {
    depthMapFbo.bindAndClear();
    glViewport(0, 0, DEPTH_MAP_WIDTH, DEPTH_MAP_HEIGHT);

    pointLightCam.position = lights[0]->getPosition();
    pointLightCam.generateTransforms();
    for (const auto& shape : shapes) {
        shape->drawToDepthMap(pointLightCam, depthShader);
    }

    Utils::unbindFboAndClear();

    shader.setCubeMapTexture(depthMapTexture, "depthMap", 5);
    shader.setFarPlane(pointLightCam.farPlane);

    glViewport(0, 0, windowWidth, windowHeight);
}

void LightController::renderForHDRAndBloom(Shader& shader, Shader& lightShader, Camera& camera) {
    hdrBloomFbo.bindAndClear();
    Shader* activeShader = &shader;
    for (const auto& shape: shapes) {
        if (shape->specialShader != nullptr) activeShader = shape->specialShader;
        shape->draw(camera, *activeShader);
    }

    activeShader = &lightShader;
    for (const auto& light: lights) {
        if (light->specialShader != nullptr) activeShader = light->specialShader;
        light->draw(camera, *activeShader);
    }

    Utils::unbindFboAndClear();
}

void LightController::adjustBrightness(float deltaTime) {
    hdrTexture.bind();
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
        float newExposure;
        if (averageBrightness <= 2.0) {
            newExposure = targetExposure;
        } else {
            newExposure = TARGET_BRIGHTNESS / log(averageBrightness);
        }
        targetExposure = newExposure;
        if (targetExposure < exposure) {
            exposure = std::max(exposure - adaptationSpeed * deltaTime, targetExposure);
        } else {
            exposure = std::min(exposure + adaptationSpeed * deltaTime, targetExposure);
        }
        //Log::log(TAG, fmt::format("exposure: {}", exposure));
        this->debugBrightness = averageBrightness;
        this->debugExposure = newExposure;

        hdrBloomShader.setExposure(exposure);

        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
    } else {
        Log::err(TAG, fmt::format("failed to read average color buffer!"));
        Utils::checkOpenGlErrors();
    }

    pbos[nextIndex].bind();

    // trigger the gpu transfer for the current frame
    // when a PBO is bound, the last argument is byte offset inside the PBO, rather than a pointer to cpu memory, it returns immediately.
    glGetTexImage(GL_TEXTURE_2D, highestMipLevel, GL_RGBA, GL_FLOAT, 0);

    Utils::unbindPbo();
}

void LightController::blurBrightAreas() {
    bool horizontal = true;
    for (int i = 0; i < blurAmount; i++) {
        blurFbos[horizontal].bind();
        blurShader.setBlurHorizontal(horizontal);
        blurResult.setTexture((i == 0) ? &bloomTexture : &blurTextures[!horizontal]);
        blurResult.draw(blurShader);
        horizontal = !horizontal;
    }
    Utils::unbindFboAndClear();
}

void LightController::renderForReal() {
    blurTextures[0].uniform = "bloomBlur";
    hdrBloomResult.setTextures({&hdrTexture, &blurTextures[0]});
    hdrBloomResult.draw(hdrBloomShader);
}

std::string LightController::getDebugString() {
    return fmt::format("average brightness: {:.3f}\nexposure: {:.3f}", debugBrightness, debugExposure);
}
    
void LightController::prepareDepthMap() {
    depthMapFbo.bind();
    depthMapFbo.attachTextureCube(depthMapTexture.ID);
    depthMapFbo.checkStatus();
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    pointLightCam.setPerspective(90.0f, 0.1f, 10.0f);

    Utils::unbindFbo();
}

void LightController::prepareHdrAndBloom() {
    // create floating point color buffer
    prepareFPTexture(hdrTexture);
    prepareFPTexture(bloomTexture);
    hdrTexture.bind();
    // create depth buffer (renderbuffer) THIS IS NEEDED TO RESOLVE DEPTHS!!! (texture only does colours)
    GLuint rboID;
    glGenRenderbuffers(1, &rboID);
    glBindRenderbuffer(GL_RENDERBUFFER, rboID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowWidth, windowHeight);
    // attach textures and buffer
    hdrBloomFbo.bind();
    hdrBloomFbo.attachTexture2D(hdrTexture.ID, 0);
    hdrBloomFbo.attachTexture2D(bloomTexture.ID, 1);
    hdrBloomFbo.attachRenderBuffer(rboID);
    // configure fbo for 2 colour attachments
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments); 
    hdrBloomFbo.checkStatus();
    Utils::unbindFbo();

    hdrBloomShader.setProjection(glm::mat4(1.0f));
    hdrBloomResult.reversedYAxis = true;
    hdrBloomResult.setCorners(-1.0f, 1.0f, 1.0f, -1.0f);
}

void LightController::prepareAvgColorBuffer() {
    for (int i = 0; i < 2; i++) {
        pbos[i].bind();
        glBufferData(GL_PIXEL_PACK_BUFFER, sizeof(float) * 4, nullptr, GL_STREAM_READ); // 4 float = 1 pixel rgba float
    }
    Utils::unbindPbo();
}

void LightController::prepareGaussianBlur() {
    for (uint i = 0; i < 2; i++) {
        prepareFPTexture(blurTextures[i]);
        blurFbos[i].bind();
        blurFbos[i].attachTexture2D(blurTextures[i].ID);
    }

    blurShader.setProjection(glm::mat4(1.0f));
    blurResult.reversedYAxis = true;
    blurResult.setCorners(-1.0f, 1.0f, 1.0f, -1.0f);
}

void LightController::prepareFPTexture(Texture& texture) {
    texture.bind();
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void LightController::calculateAttenuationCoefficients(float range, float* linear, float* quadratic) {
    *linear = LINEAR_COEFFICIENT * pow(range, LINEAR_POWER);
    *quadratic = QUADRATIC_COEFFICIENT * pow(range, QUADRATIC_POWER);
}

