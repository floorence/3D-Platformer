#ifndef PLAYER_H
#define PLAYER_H

#include"Mass.h"
#include"camera/Camera.h"
#include "controller/SettingsListener.h"
#include "shape/Line.h"
#include "shape/RectangularPrism.h"

class Player: public Mass, public SettingsListener {
public:
    Camera camera;
	Camera thirdPersonCam;
	RectangularPrism body;
	Shader lineShader;
	Line orientationLine;
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 thirdPersonOrientation = glm::vec3(0.0f, 0.0f, -1.0f);

    Player(glm::vec3 position, int windowWidth, int windowHeight);

	void setTextures(AssetTexture* diffuse, AssetTexture* specular);
	std::vector<Shape3D*> getShapes();
	Camera* getActiveCamera();
	std::string getDebugString() override;
	
	// inputs
	void handleKeyInputs(GLFWwindow* window, float deltaTime);
	void handleKeyInputs(GLFWwindow* window, int key, int action);
	void handleMousePos(GLFWwindow* window, double xpos, double ypos);
	void handleMouseScroll(GLFWwindow* window, double xoffset, double yoffset);

    void onSettingsChanged(const Settings& settings) override;
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
	const float MIN_THIRD_PERSON_DIST = 0.2f;

	void handleFocusChange(GLFWwindow* window);
	glm::vec3* getActiveOrientation();
	void syncCamerasAndBody(glm::vec3 movement);
};

#endif
