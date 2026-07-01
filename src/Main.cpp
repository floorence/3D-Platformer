#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<fmt/format.h>

#include "shape/DebugPyramid.h"
#include"shape/Sphere.h"
#include"shape/RectangularPrism.h"
#include"controller/LightController.h"
#include"texture/FontTexture.h"
#include"texture/ImageTexture.h"
#include"gui/Gui.h"
#include"util/Log.h"

const unsigned int width = 800;
const unsigned int height = 800;
const std::string TAG = "Main";

Camera* camera_ptr;

void mouseCallback(GLFWwindow*, double xpos, double ypos) {
	camera_ptr->handleMousePos(xpos, ypos);
}

void keyCallback(GLFWwindow* window, int key, int, int action, int) {
	camera_ptr->handleKeyInputs(window, key, action);
}

std::string formatPerformanceInfo(float frameTime, float realFrameTime) {
	int fps = 1 / frameTime;
	int realFps = 1 / realFrameTime;

	return fmt::format("FPS: {}  |  {}\nframe time: {:.3f}  |  {:.3f}", fps, realFps, frameTime * 1000, realFrameTime * 1000);
}

int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "window", NULL, NULL);

	if (window == NULL) {
		Log::log(TAG, "Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);  // enable VSync

	gladLoadGL();

	// on linux, framebuffer size and window size are not always identical
	int fbWidth, fbHeight;
	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

	Log::log(TAG, fmt::format("configuring viewport: 0, 0, {}, {}", fbWidth, fbHeight));

	glViewport(0, 0, fbWidth, fbHeight);

	Log::log(TAG, "opengl initialized");

	// make gui
	FontTexture guiDiffuse = FontTexture("assets/pixel_operator_short_dollar.ttf");
	Gui gui(&guiDiffuse, width, height);

	// make debug pyramid
	Texture planksDiffuse = ImageTexture("assets/planks.png", TextureType::Diffuse);
	Texture planksSpecular = ImageTexture("assets/planks.png", TextureType::Diffuse, GL_UNSIGNED_BYTE, true);
	DebugPyramid pyramid(&planksDiffuse, &planksSpecular, glm::vec3(0.0f, 0.0f, 0.0f));

	// make sphere
	Sphere sphere(&planksDiffuse, &planksSpecular, glm::vec3(3.0f, 0.0f, 0.0f), 1);

	// make rectangular prism
	Texture bunDiffuse = ImageTexture("assets/metal.jpg", TextureType::Diffuse);
	Texture bunSpecular = ImageTexture("assets/metal.jpg", TextureType::Diffuse, GL_UNSIGNED_BYTE, true);
	RectangularPrism rect(&bunDiffuse, &bunSpecular, glm::vec3(-3.0f, 0.0f, 0.0f), 0.5f, 1.0f, 0.75f);
	rect.setRotation(0, 0, 180);

	// make light cube
	RectangularPrism light(nullptr, nullptr, glm::vec3(0.5f, 0.5f, 0.5f), 0.2f, 0.2f, 0.2f, true);
	light.setColor(glm::vec3(1.0f, 1.0f, 1.0f), 5.0f);

	LightController lc;
	lc.registerShapes({&pyramid, &sphere, &rect, &light});
	lc.processLighting();

	Log::log(TAG, "initial lighting processing completed");

	glEnable(GL_DEPTH_TEST); // enable depth buffer so that stuff in front blocks stuff behind it

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
	camera_ptr = &camera;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float lastUpdatedInfoText = 0.0f;
	std::string currInfoText = "";

	// first: total frame time, second: number of frames. real frame time is the frame time if vsync wasn't on.
	std::pair<float, int> totalFrameTime(0.0f, 0);
	std::pair<float, int> totalRealFrameTime(0.0f, 0);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetKeyCallback(window, keyCallback);

	Log::log(TAG, "everything is set up; starting main game loop");

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // background colour
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the back buffer

		camera.handleKeyInputs(window, deltaTime);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		pyramid.draw(camera);
		light.draw(camera);
		sphere.draw(camera);
		rect.draw(camera);
		gui.drawText(camera.getDebugString(), 200, 200, 400, 20);

		glfwPollEvents();

		// doing this here since glfwSwapBuffers() is what actually suspends when using vsync

		float realCurrentFrame = glfwGetTime();
		totalFrameTime.first += deltaTime;
		totalFrameTime.second++;
		totalRealFrameTime.first += realCurrentFrame - currentFrame;
		totalRealFrameTime.second++;

		if (realCurrentFrame - lastUpdatedInfoText >= 1.0f) {
			lastUpdatedInfoText = realCurrentFrame;
			float avgFrameTime = totalFrameTime.first / totalFrameTime.second;
			float avgRealFrameTime = totalRealFrameTime.first / totalRealFrameTime.second;
			
			currInfoText = formatPerformanceInfo(avgFrameTime, avgRealFrameTime); 
			totalFrameTime = std::pair(0.0f, 0);
			totalRealFrameTime = std::pair(0.0f, 0);
		}

		gui.drawText(currInfoText, width - 200, 10, 200, 16);
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}