#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"shader/Shader.h"

class Camera {
public:
	glm::vec3 position;
	glm::mat4 projection;
	glm::mat4 view;
	int width, height;

	// constants
	const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 DOWN = glm::vec3(0.0f, -1.0f, 0.0f);

	Camera(glm::vec3 position, int width, int height);

	void setPerspective(float FOVdeg, float nearPlane, float farPlane);
	void lookAt(glm::vec3 orientation);
	void lookAt(glm::vec3 orientation, glm::vec3 up);
	glm::mat4 getCameraMatrix();
	/** @brief give shader the position and cameraMatrix, caller has to activate shader beforehand */
	virtual void exportCamera(Shader& shader);
protected:
	float farPlane;
private:
	const std::string TAG = "Camera";
};

#endif