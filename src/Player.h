#ifndef PLAYER_H
#define PLAYER_H

#include"Mass.h"
#include"camera/Camera.h"
#include "controller/SettingsListener.h"
#include "shape/Model.h"
#include "shape/Line.h"

enum class FlightMode {
	Tilt, Turn
};

class Player: public Mass, public SettingsListener, public Drawable3D {
public:
    Camera camera;
	Camera thirdPersonCam;
	Model body;
	Line orientationLine;
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f); // should always be normalized!!!
	glm::vec3 thirdPersonOrientation = glm::vec3(0.0f, 0.0f, -1.0f); // length should be the same as thirdPersonDist

    Player(glm::vec3 position, int windowWidth, int windowHeight);

	Camera* getActiveCamera();
	std::string getDebugString() override;
	
	// inputs
	void handleKeyInputs(GLFWwindow* window, float deltaTime);
	void handleKeyInputs(GLFWwindow* window, int key, int action);
	void handleMousePos(GLFWwindow* window, double xpos, double ypos);
	void handleMouseScroll(GLFWwindow* window, double xoffset, double yoffset);

    void onSettingsChanged(const Settings& settings) override;

	void draw(Camera& camera, Shader& shader) override;
	void drawToDepthMap(PointLightCamera& camera, Shader& depthShader) override;
private:
	const float ACCELERATION_MULTIPLIER = 1.0f;
	const float TILT_TURN_SPEED = glm::radians(45.0f); // radians per second
	const float TILT_MAX = glm::radians(45.0f); // radians
	float yaw = 0.0f; // radians
	float pitch = 0.0f; // radians
	float roll = 0.0f; // radians

	bool firstClick = true;
	bool focused = true;
	double lastX, lastY;

	float sensitivity = 100.0f; 

	FlightMode flightMode = FlightMode::Tilt;
	// first: total force applied in the past frames, second: number of frames
	std::pair<glm::vec3, int> totalForce = std::pair(glm::vec3(0.0f), 0);

	bool thirdPerson = false;
	float thirdPersonDist = 1.0f; // distance from third person cam to player
	const float MIN_THIRD_PERSON_DIST = 0.2f;

	void handleFocusChange(GLFWwindow* window);
	glm::vec3* getActiveOrientation();
	/** yawTurn and pitchTurn are 0, -1, or 1 depending on direction */
	void syncCamerasAndBody(glm::vec3 movement, int yawTurn, int pitchTurn, float deltaTime);
};

#endif
