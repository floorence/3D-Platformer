#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

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
	float farPlane;

	// constants
	static constexpr glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
	static constexpr glm::vec3 DOWN = glm::vec3(0.0f, -1.0f, 0.0f);
	static constexpr glm::vec3 FORWARD = glm::vec3(0.0f, 0.0f, -1.0f);

	Camera(glm::vec3 position, int windowWidth, int windowHeight);

	void setPerspective(float FOVdeg, float nearPlane, float farPlane);
	void lookAt(glm::vec3 orientation, glm::vec3 up = UP);
	glm::mat4 getCameraMatrix();
private:
	const std::string TAG = "Camera";
};

#endif