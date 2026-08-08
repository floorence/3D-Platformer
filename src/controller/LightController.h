#ifndef LIGHT_CONTROLLER_H
#define LIGHT_CONTROLLER_H

#include "buffer/FBO.h"
#include "buffer/PBO.h"
#include "controller/SettingsListener.h"
#include "gui/Quad.h"
#include "shape/Shape3D.h"

class LightController: public SettingsListener {
public:
    LightController(int windowWidth, int windowHeight);

    void registerShape(Shape3D* shape);
    void registerShapes(const std::vector<Shape3D*>& shapes);
    void processLighting(Shader& shader);

    // these functions must all be called in order each frame
    void renderForShadows(Shader& shader);
    void renderForHDRAndBloom(Shader& shader, Shader& lightShader, Camera& camera);
    void adjustBrightness(float deltaTime);
    void blurBrightAreas();
    void renderForReal();

    void onSettingsChanged(const Settings& settings) override;
    std::string getDebugString();
private:
    int windowWidth, windowHeight;
    std::vector<Shape3D*> lights;
    std::vector<Shape3D*> shapes;

    const std::string TAG = "LightController";

    // stuff for shadows
    const uint DEPTH_MAP_WIDTH = 1024, DEPTH_MAP_HEIGHT = 1024;
    FBO depthMapFbo;
    CubeMapTexture depthMapTexture;
    Shader depthShader;
    PointLightCamera pointLightCam;

    // stuff for hdr and bloom
    FBO hdrBloomFbo;
    Texture hdrTexture;
    Texture bloomTexture;
    Shader hdrBloomShader;
    Quad hdrBloomResult;
    // average colour buffer
    PBO pbos[2];
    int pboIndex = 0;
    float exposure = 1.0f;
    float targetExposure = 1.0f;
    float adaptationDelay = 0.5f; // seconds
    float adaptationSpeed = 0.2f; // exposure per second
    const float TARGET_BRIGHTNESS = 0.18f;
    // gaussian blur
    FBO blurFbos[2];
    Texture blurTextures[2];
    Shader blurShader;
    Quad blurResult;
    int blurAmount = 10;

    // debug vars
    float debugBrightness;
    float debugExposure;

    // these are parameters for a best fit power regression model on the values that work well for specified ranges,
    // courtesy of https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
    // thank you desmos for fitting the model (^_^)
    const float LINEAR_COEFFICIENT = 4.88011;
    const float LINEAR_POWER = -1.02046;
    const float QUADRATIC_COEFFICIENT = 87.39333;
    const float QUADRATIC_POWER = -2.03568;

    void prepareDepthMap();
    void prepareHdrAndBloom();
    void prepareAvgColorBuffer();
    void prepareGaussianBlur();

    void prepareFPTexture(Texture& texture); // FP = floating point
    /**
     * @param range the distance away from the light that it can (visibly) reach.
     *              note that most of the light falls in the first 20% of range.
     */
    void calculateAttenuationCoefficients(float range, float* linear, float* quadratic);
};

#endif