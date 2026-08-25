#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<fmt/format.h>

#include "controller/ClickController.h"
#include "gui/Button.h"
#include "gui/SettingsMenu.h"
#include "shape/Model.h"
#include "shape/DebugPyramid.h"
#include"shape/Sphere.h"
#include"shape/RectangularPrism.h"
#include"controller/LightController.h"
#include "shape/Trail.h"
#include"texture/FontTexture.h"
#include"texture/ImageTexture.h"
#include "util/Globals.h"
#include"util/Log.h"
#include"Player.h"
#include "util/Utils.h"

const unsigned int width = 800;
const unsigned int height = 800;
const std::string TAG = "Main";

Player* player_ptr;
ClickController* clickController_ptr;

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	player_ptr->handleMousePos(window, xpos, ypos);
	clickController_ptr->handleMousePos(xpos, ypos);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	player_ptr->handleMouseScroll(window, xoffset, yoffset);
}

void keyCallback(GLFWwindow* window, int key, int, int action, int) {
	player_ptr->handleKeyInputs(window, key, action);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
		clickController_ptr->handleMouseButton(xpos, ypos, action);
    }
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
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

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

	// on some displays, framebuffer size and window size are not always the same
	int fbWidth, fbHeight;
	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

	std::vector<Shape3D*> objects;

	// make debug pyramid
	AssetTexture planksDiffuse = ImageTexture("assets/planks.png", TextureType::Diffuse);
	AssetTexture planksSpecular = ImageTexture("assets/planks.png", TextureType::Specular, GL_UNSIGNED_BYTE, true);
	// DebugPyramid pyramid(&planksDiffuse, &planksSpecular, glm::vec3(0.0f, 0.0f, 0.0f));
	// objects.push_back(&pyramid);

	// make sphere
	Sphere sphere(&planksDiffuse, &planksSpecular, glm::vec3(2.0f, 0.0f, 0.0f), 0.2f);
	objects.push_back(&sphere);

	// make rectangular prism
	AssetTexture bunDiffuse = ImageTexture("assets/metal.jpg", TextureType::Diffuse);
	AssetTexture bunSpecular = ImageTexture("assets/metal.jpg", TextureType::Specular, GL_UNSIGNED_BYTE, true);
	RectangularPrism rect(&bunDiffuse, &bunSpecular, glm::vec3(-2.0f, 0.0f, 0.0f), 0.5f, 1.0f, 0.75f);
	rect.setRotation(0, 0, glm::radians(180.0f));
	objects.push_back(&rect);

	// make floor
	AssetTexture floorDiffuse = ImageTexture("assets/stone.jpg", TextureType::Diffuse);
	AssetTexture floorSpecular = ImageTexture("assets/stone.jpg", TextureType::Specular, GL_UNSIGNED_BYTE, true);
	RectangularPrism floor(&floorDiffuse, &floorSpecular, glm::vec3(0.0f, -1.2f, 0.0f), 5.0f, 0.1f, 5.0f);
	objects.push_back(&floor);

	/*
	RectangularPrism rightWall(&floorDiffuse, &floorSpecular, glm::vec3(2.5f, 1.3f, 0.0f), 0.1f, 5.0f, 5.0f);
	RectangularPrism leftWall(&floorDiffuse, &floorSpecular, glm::vec3(-2.5f, 1.3f, 0.0f), 0.1f, 5.0f, 5.0f);
	RectangularPrism backWall(&floorDiffuse, &floorSpecular, glm::vec3(0.0f, 1.3f, 2.5f), 5.0f, 5.0f, 0.1f);
	RectangularPrism frontWall(&floorDiffuse, &floorSpecular, glm::vec3(0.0f, 1.3f, -2.5f), 5.0f, 5.0f, 0.1f);

	objects.push_back(&rightWall);
	objects.push_back(&leftWall);
	objects.push_back(&backWall);
	objects.push_back(&frontWall);
    */

	RectangularPrism floorCube(&planksDiffuse, &planksSpecular, glm::vec3(-1.0f, -0.95f, -1.0f), 0.4f, 0.4f, 0.4f);
	objects.push_back(&floorCube);

	RectangularPrism floorLight(&planksDiffuse, &planksSpecular, glm::vec3(0.0f, -0.8f, 0.0f), 0.2f, 0.2f, 0.2f, true);
	floorLight.setColor(glm::vec3(100.0f, 100.0f, 100.0f));
	objects.push_back(&floorLight);

	// make light cube
	// RectangularPrism light(nullptr, nullptr, glm::vec3(0.5f, 0.5f, 0.5f), 0.2f, 0.2f, 0.2f, true);
	// light.setColor(glm::vec3(1.0f, 1.0f, 1.0f), 7.0f);
	// objects.push_back(&light);

	Player player(glm::vec3(0.0f, 0.0f, 2.0f), width, height);
	player_ptr = &player;

	// test trail no rotating
	Trail trail(glm::vec3(0.0f, 0.0f, 0.0f), 10);
	trail.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
	for (int i = 0; i < 10; i++) {
		trail.addPoint(glm::vec3(0.0f, 0.0f, 0.1f * i), 0.0f);
	}	

	Shader shader("shader/default.vert", "shader/default.frag");
	Shader lightShader("shader/light.vert", "shader/light.frag");
	Shader flatShader("shader/default.vert", "shader/gui.frag");
	Shader guiShader("shader/gui.vert", "shader/gui.frag");
	Shader fontShader("shader/gui.vert", "shader/font.frag");
	glm::mat4 guiProjection = glm::ortho(0.0f, (float)width, (float)width, 0.0f, -1.0f, 1.0f);	
	guiShader.setProjection(guiProjection);
	fontShader.setProjection(guiProjection);

	Globals::DefaultShader = &shader;
	Globals::LightShader = &lightShader;
	Globals::FlatShader = &flatShader;
	Globals::GuiShader = &guiShader;
	Globals::FontShader = &fontShader;

	LightController lc(fbWidth, fbHeight);
	lc.registerShapes(objects);
	lc.registerDrawable(&trail);
	lc.registerDrawable(&player);
	lc.processLighting(shader);

	Log::log(TAG, "initial lighting processing completed");

	// make text
	FontTexture fontTex("assets/pixel_operator_short_dollar.ttf");
	Globals::Font = &fontTex;
	// TextRenderer tr(width, height);
	Text playerDebugText;
	playerDebugText.setBounds(10, 10, 400, 200);
	playerDebugText.setFontSize(20);
	playerDebugText.setCenterText(false);

	Text performanceText;
	performanceText.setBounds(width - 200, 10, 200, 100);
	performanceText.setFontSize(16);
	performanceText.setCenterText(false);

	SettingsController sc;
	SettingsMenu settingsMenu(&sc);
	settingsMenu.setCorners(100, 100, width - 100, height - 100);
	sc.registerListeners({&settingsMenu, &lc, &player});
	sc.load();

	Log::log(TAG, "settings loaded from save");

	Button button;
	button.setCorners(width - 80, height - 40, width - 10, height - 10);
	button.setText("settings");
	button.setBackgroundColor(glm::vec3(1.0f, 0.71f, 0.957f));
	button.setOnClick([&settingsMenu]() {
		settingsMenu.isOpen = !settingsMenu.isOpen;
	});

	ClickController cc;
	clickController_ptr = &cc;
	cc.registerClickable(&button);
	cc.registerClickable(&settingsMenu);
	
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float lastUpdatedInfoText = 0.0f;

	// first: total frame time, second: number of frames. real frame time is the frame time if vsync wasn't on.
	std::pair<float, int> totalFrameTime(0.0f, 0);
	std::pair<float, int> totalRealFrameTime(0.0f, 0);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	Log::log(TAG, fmt::format("configuring viewport: 0, 0, {}, {}", fbWidth, fbHeight));

	glViewport(0, 0, fbWidth, fbHeight);
	glEnable(GL_DEPTH_TEST); // enable depth buffer so that stuff in front blocks stuff behind it
	glEnable(GL_CULL_FACE); // enable back face culling

	Log::log(TAG, "everything is set up; starting main game loop");

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // background colour

		player.handleKeyInputs(window, deltaTime);
		lc.renderForShadows(shader);
		lc.renderForHDRAndBloom(*player.getActiveCamera());
		lc.adjustBrightness(deltaTime);
		lc.blurBrightAreas();
		lc.renderForReal();

		glDisable(GL_DEPTH_TEST);
		playerDebugText.setText(player.getDebugString());
		playerDebugText.draw();
		// tr.drawText(lc.getDebugString(), fontShader, 10, 100, 400, 20, glm::vec3(1.0f, 0.0f, 0.0f));
		button.draw();

		if (settingsMenu.isOpen) settingsMenu.draw();

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
			
			performanceText.setText(formatPerformanceInfo(avgFrameTime, avgRealFrameTime)); 
			totalFrameTime = std::pair(0.0f, 0);
			totalRealFrameTime = std::pair(0.0f, 0);
		}

		performanceText.draw();
		glEnable(GL_DEPTH_TEST);

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}