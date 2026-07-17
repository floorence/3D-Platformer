#ifndef LIGHT_CONTROLLER_H
#define LIGHT_CONTROLLER_H

#include "buffer/FBO.h"
#include "buffer/PBO.h"
#include "gui/Quad.h"
#include "shape/Shape3D.h"

class LightController {
public:
    LightController(int windowWidth, int windowHeight);

    void registerShape(Shape3D* shape);
    void registerShapes(std::vector<Shape3D*> shapes);
    void processLighting(Shader& shader);

    // these functions must all be called in order each frame
    void renderForShadows(Shader& shader);
    void renderForHDR(Shader& shader, Shader& lightShader, Camera& camera);
    void adjustBrightness(float deltaTime);
    void renderForReal();

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

    // stuff for hdr
    FBO hdrFbo;
    Texture colorBufTexture;
    Shader hdrShader;
    Quad hdrResult;
    // average colour buffer
    PBO pbos[2];
    int pboIndex = 0;
    float exposure = 1.0f;
    float targetExposure = 1.0f;
    float adaptationSpeed = 0.2f; // exposure per second
    const float TARGET_BRIGHTNESS = 0.18f;

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
    void prepareHDR();
    void prepareAvgColorBuffer();
    /**
     * @param range the distance away from the light that it can (visibly) reach.
     *              note that most of the light falls in the first 20% of range.
     */
    void calculateAttenuationCoefficients(float range, float* linear, float* quadratic);
};

#endif