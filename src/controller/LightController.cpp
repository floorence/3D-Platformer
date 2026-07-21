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

void LightController::renderForHDRAndBloom(Shader& shader, Shader& lightShader, Camera& camera) {
    hdrBloomFbo.bindAndClear();

    for (const auto& shape: shapes) {
        shape->draw(camera, shader);
    }
    for (const auto& light: lights) {
        light->draw(camera, lightShader);
    }

    hdrBloomFbo.unbindAndClear();
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

    pbos[nextIndex].unbind();
}

void LightController::blurBrightAreas() {
    bool horizontal = true;
    for (int i = 0; i < blurAmount; i++) {
        blurFbos[horizontal].bind();
        blurShader.setBlurHorizontal(horizontal);
        blurResult.setTexture((i == 0) ? &bloomTexture : &blurTextures[!horizontal]);
        blurResult.draw(blurShader, -1.0f, 1.0f, 1.0f, -1.0f);
        horizontal = !horizontal;
    }
    blurFbos[0].unbindAndClear();
}

void LightController::renderForReal() {
    blurTextures[0].uniform = "bloomBlur";
    hdrBloomShader.setTexture(hdrTexture, 0); // TODO not needed since mesh already sets the texture
    hdrBloomShader.setTexture(blurTextures[0], 1);
	hdrBloomShader.setProjection(glm::mat4(1.0f));
    hdrBloomResult.setTexture(&hdrTexture);
    hdrBloomResult.draw(hdrBloomShader, -1.0f, 1.0f, 1.0f, -1.0f);
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

    depthMapFbo.unbind();
}

void LightController::prepareHdrAndBloom() {
    // create floating point color buffer
    prepareFPTexture(hdrTexture);
    prepareFPTexture(bloomTexture);
    hdrTexture.bind();
    glGenerateMipmap(GL_TEXTURE_2D); // for getting average colour later TODO: might not be needed here since its called every frame anyway
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
    hdrBloomFbo.unbind();
}

void LightController::prepareAvgColorBuffer() {
    for (int i = 0; i < 2; i++) {
        pbos[i].bind();
        glBufferData(GL_PIXEL_PACK_BUFFER, sizeof(float) * 4, nullptr, GL_STREAM_READ); // 4 float = 1 pixel rgba float
        pbos[i].unbind();
    }
}

void LightController::prepareGaussianBlur() {
    for (uint i = 0; i < 2; i++) {
        prepareFPTexture(blurTextures[i]);
        blurFbos[i].bind();
        blurFbos[i].attachTexture2D(blurTextures[i].ID);
    }
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

