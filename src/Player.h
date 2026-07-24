#ifndef PLAYER_H
#define PLAYER_H

#include"Mass.h"
#include"camera/Camera.h"
#include "shape/RectangularPrism.h"

class Player: public Mass {
public:
    Camera camera;
	Camera thirdPersonCam;
	RectangularPrism body;
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 thirdPersonOrientation = glm::vec3(0.0f, 0.0f, -1.0f);

    Player(glm::vec3 position, int windowWidth, int windowHeight);

	void setTextures(AssetTexture* diffuse, AssetTexture* specular);
	Camera* getActiveCamera();
	std::string getDebugString() override;
	
	// inputs
	void handleKeyInputs(GLFWwindow* window, float deltaTime);
	void handleKeyInputs(GLFWwindow* window, int key, int action);
	void handleMousePos(GLFWwindow* window, double xpos, double ypos);
private:
	const float MAX_SPEED_DEFAULT = 1.0f;
	const float MAX_SPEED_SPRINTING = 2.0f;
	const float ACCELERATION_MULTIPLIER = 1.0f;

	bool firstClick = true;
	bool focused = true;

	double lastX, lastY;

	// TODO
	float maxSpeed = MAX_SPEED_DEFAULT; // units per second
	float sensitivity = 100.0f; 

	bool thirdPerson = false;
	float thirdPersonDist = 1.0f; // distance from third person cam to player

	void handleFocusChange(GLFWwindow* window);
	glm::vec3* getActiveOrientation();
	void syncCamerasAndBody(glm::vec3 movement);
};

#endif
