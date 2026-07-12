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
#include"gui/TextRenderer.h"
#include"util/Log.h"
#include"Player.h"

const unsigned int width = 800;
const unsigned int height = 800;
const std::string TAG = "Main";

Player* player_ptr;

void mouseCallback(GLFWwindow*, double xpos, double ypos) {
	player_ptr->handleMousePos(xpos, ypos);
}

void keyCallback(GLFWwindow* window, int key, int, int action, int) {
	player_ptr->handleKeyInputs(window, key, action);
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

	Log::log(TAG, "opengl initialized");

	// on linux, framebuffer size and window size are not always identical
	int fbWidth, fbHeight;
	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

	// make font renderer
	FontTexture font = FontTexture("assets/pixel_operator_short_dollar.ttf");
	TextRenderer tr(&font);

	std::vector<Shape3D*> objects;

	// make debug pyramid
	AssetTexture planksDiffuse = ImageTexture("assets/planks.png", TextureType::Diffuse);
	AssetTexture planksSpecular = ImageTexture("assets/planks.png", TextureType::Specular, GL_UNSIGNED_BYTE, true);
	DebugPyramid pyramid(&planksDiffuse, &planksSpecular, glm::vec3(0.0f, 0.0f, 0.0f));
	objects.push_back(&pyramid);

	// make sphere
	Sphere sphere(&planksDiffuse, &planksSpecular, glm::vec3(2.0f, 0.0f, 0.0f), 0.2f);
	objects.push_back(&sphere);

	// make rectangular prism
	AssetTexture bunDiffuse = ImageTexture("assets/metal.jpg", TextureType::Diffuse);
	AssetTexture bunSpecular = ImageTexture("assets/metal.jpg", TextureType::Specular, GL_UNSIGNED_BYTE, true);
	RectangularPrism rect(&bunDiffuse, &bunSpecular, glm::vec3(-2.0f, 0.0f, 0.0f), 0.5f, 1.0f, 0.75f);
	rect.setRotation(0, 0, 180);
	objects.push_back(&rect);

	// make floor
	AssetTexture floorDiffuse = ImageTexture("assets/stone.jpg", TextureType::Diffuse);
	AssetTexture floorSpecular = ImageTexture("assets/stone.jpg", TextureType::Specular, GL_UNSIGNED_BYTE, true);
	RectangularPrism floor(&floorDiffuse, &floorSpecular, glm::vec3(0.0f, -1.2f, 0.0f), 5.0f, 0.1f, 5.0f);
	objects.push_back(&floor);

	RectangularPrism rightWall(&floorDiffuse, &floorSpecular, glm::vec3(2.5f, 1.3f, 0.0f), 0.1f, 5.0f, 5.0f);
	RectangularPrism leftWall(&floorDiffuse, &floorSpecular, glm::vec3(-2.5f, 1.3f, 0.0f), 0.1f, 5.0f, 5.0f);
	RectangularPrism backWall(&floorDiffuse, &floorSpecular, glm::vec3(0.0f, 1.3f, 2.5f), 5.0f, 5.0f, 0.1f);
	RectangularPrism frontWall(&floorDiffuse, &floorSpecular, glm::vec3(0.0f, 1.3f, -2.5f), 5.0f, 5.0f, 0.1f);

	objects.push_back(&rightWall);
	objects.push_back(&leftWall);
	objects.push_back(&backWall);
	objects.push_back(&frontWall);

	RectangularPrism floorCube(&planksDiffuse, &planksSpecular, glm::vec3(-1.0f, -0.95f, -1.0f), 0.4f, 0.4f, 0.4f);
	objects.push_back(&floorCube);

	RectangularPrism floorLight(&planksDiffuse, &planksSpecular, glm::vec3(0.0f, -0.8f, 0.0f), 0.2f, 0.2f, 0.2f, true);
	floorLight.setColor(glm::vec3(100.0f, 100.0f, 100.0f), 7.0f);
	objects.push_back(&floorLight);

	// make light cube
	// RectangularPrism light(nullptr, nullptr, glm::vec3(0.5f, 0.5f, 0.5f), 0.2f, 0.2f, 0.2f, true);
	// light.setColor(glm::vec3(1.0f, 1.0f, 1.0f), 7.0f);
	// objects.push_back(&light);

	// make shaders and light controller
	Shader shader("shader/default.vert", "shader/default.frag");
	Shader lightShader("shader/light.vert", "shader/light.frag");
	Shader fontShader("shader/gui.vert", "shader/font.frag");
	glm::mat4 guiProjection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);	
	fontShader.setProjection(guiProjection);

	LightController lc(fbWidth, fbHeight);
	lc.registerShapes(objects);
	lc.processLighting(shader);

	Log::log(TAG, "initial lighting processing completed");

	glEnable(GL_DEPTH_TEST); // enable depth buffer so that stuff in front blocks stuff behind it
	glEnable(GL_CULL_FACE); // enable face culling, mostly so that we can cull front faces for depth map rendering to avoid peter-panning

	Player player(glm::vec3(0.0f, 0.0f, 2.0f), width, height);
	player_ptr = &player;

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

	Log::log(TAG, fmt::format("configuring viewport: 0, 0, {}, {}", fbWidth, fbHeight));

	glViewport(0, 0, fbWidth, fbHeight);

	Log::log(TAG, "everything is set up; starting main game loop");

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // background colour

		player.handleKeyInputs(window, deltaTime);
		lc.renderForShadows(shader);
		lc.renderForHDR(shader, lightShader, player.camera);
		lc.renderForReal();

		glDisable(GL_DEPTH_TEST);
		tr.drawText(player.getDebugString(), fontShader, 10, 10, 400, 20);

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

		tr.drawText(currInfoText, fontShader, width - 200, 10, 200, 16);
		glEnable(GL_DEPTH_TEST);

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}