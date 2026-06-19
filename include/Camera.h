#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"Shader.h"

class Camera {
public:
	Camera(int width, int height, glm::vec3 position);

	void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
	void exportCamera(Shader& shader, const char* posUniform, const char* matUniform); // give shader the position and cameraMatrix, caller has to activate shader beforehand

	// inputs
	void handleKeyInputs(GLFWwindow* window, float deltaTime);
	void handleKeyInputs(GLFWwindow* window, int key, int action);
	void handleMousePos(double xpos, double ypos);
private:
	glm::vec3 position;
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);	// units per second
	glm::mat4 cameraMatrix = glm::mat4(1.0f);

	const float MAX_SPEED_DEFAULT = 1.0f;
	const float MAX_SPEED_SPRINTING = 2.0f;
	const float ACCELERATION_MULTIPLIER = 1.0f;
	const float AIR_RESISTANCE_MULTIPLIER = 2.0f;
	const float STOPPING_SPEED = 0.0005f;

	bool firstClick = true;
	bool focused = true;

	int width;
	int height;

	double lastX;
	double lastY;

	float maxSpeed = MAX_SPEED_DEFAULT; // units per second
	float sensitivity = 100.0f; 

	// constants
	const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
	const std::string TAG = "Camera";

	void handleFocusChange(GLFWwindow* window);
	void applyAcceleration(glm::vec3 a, float dt);
	glm::vec3 getAirResistance(); // return acceleration from air resistance
	void processVelocity();
};

#endif