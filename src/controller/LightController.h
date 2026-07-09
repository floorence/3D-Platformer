#ifndef LIGHT_CONTROLLER_H
#define LIGHT_CONTROLLER_H

#include "shape/Shape3D.h"

class LightController {
public:
    LightController(int windowWidth, int windowHeight);

    void registerShape(Shape3D* shape);
    void registerShapes(std::vector<Shape3D*> shapes);
    void processLighting(Shader& shader);
    void processShadows(Shader& shader);
    void processHDR(Shader& shader);
private:
    int windowWidth, windowHeight;
    std::vector<Shape3D*> lights;
    std::vector<Shape3D*> shapes;

    const std::string TAG = "LightController";

    // stuff for shadows
    const uint DEPTH_MAP_WIDTH = 1024, DEPTH_MAP_HEIGHT = 1024;
    GLuint depthMapFboID = 0;
    CubeMapTexture depthMapTexture;
    Shader depthShader;
    PointLightCamera pointLightCam;

    // stuff for hdr
    GLuint hdrFboID = 0;
    

    // these are parameters for a best fit power regression model on the values that work well for specified ranges,
    // courtesy of https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
    // thank you desmos for fitting the model (^_^)
    const float LINEAR_COEFFICIENT = 4.88011;
    const float LINEAR_POWER = -1.02046;
    const float QUADRATIC_COEFFICIENT = 87.39333;
    const float QUADRATIC_POWER = -2.03568;

    void initDepthMap();
    void initColorBuffer();
    /**
     * @param range the distance away from the light that it can (visibly) reach.
     *              note that most of the light falls in the first 20% of range.
     */
    void calculateAttenuationCoefficients(float range, float* linear, float* quadratic);
};

#endif