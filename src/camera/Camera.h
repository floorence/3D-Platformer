#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include "util/Constants.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include <string>

class Camera {
public:
	glm::vec3 position;
	glm::mat4 projection;
	glm::mat4 view;
	int windowWidth, windowHeight;
	float fovDeg, nearPlane, farPlane;

	Camera(glm::vec3 position, int windowWidth, int windowHeight);

	void setPerspective(float fovDeg, float nearPlane, float farPlane);
	void setViewSize(float width, float height);
	void lookAt(glm::vec3 orientation, glm::vec3 up = Constants::UP);
	glm::mat4 getCameraMatrix();
private:
	const std::string TAG = "Camera";
};

#endif