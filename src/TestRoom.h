#pragma once

#include "3d/shape/RectangularPrism.h"
#include "3d/shape/Shape3D.h"
#include "3d/shape/Sphere.h"
#include "texture/ImageTexture.h"

class TestRoom {
public:
	std::vector<Shape3D*> objects;

    TestRoom();
private:
    ImageTexture planksDiffuse;
	ImageTexture planksSpecular;
	ImageTexture metalDiffuse;
	ImageTexture metalSpecular;
	ImageTexture stoneDiffuse;
	ImageTexture stoneSpecular;

    Sphere sphere;
    RectangularPrism rect;
    RectangularPrism floor;
    RectangularPrism rightWall;
    RectangularPrism leftWall;
    RectangularPrism floorCube;
    RectangularPrism floorLight;
};