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
	Shader lineShader;
	Line orientationLine;
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 thirdPersonOrientation = glm::vec3(0.0f, 0.0f, -1.0f);

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
	const float TILT_TURN_SPEED = 45.0f; // degrees per second
	const float TILT_MAX = 45.0f;
	float tilt = 0.0f; // degrees

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
	void syncCamerasAndBody(glm::vec3 movement, float turnAmount, float deltaTime);
};

#endif
