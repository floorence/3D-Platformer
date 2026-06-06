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
	glm::vec3 position;

	// Camera constructor to set up initial values
	Camera(int width, int height, glm::vec3 position);

	// Updates and exports the camera matrix to the Vertex Shader
	void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
	void exportMatrix(Shader& shader, const char* uniform);
	// Handles camera inputs
	void handleInputs(GLFWwindow* window, float deltaTime);
private:
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);

	// Prevents the camera from jumping around when first clicking left click
	bool firstClick = true;

	// Stores the width and height of the window
	int width;
	int height;

	// Adjust the speed of the camera and it's sensitivity when looking around
	float speed = 1.0f;
	float sensitivity = 1.0f;

	// constants
	const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
};

#endif