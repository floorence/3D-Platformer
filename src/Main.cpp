#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<fmt/format.h>

#include "Window.h"
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

	// create shaders, textures, and populate globals
	Shader shader("shader/default.vert", "shader/default.frag");
	Shader lightShader("shader/light.vert", "shader/light.frag");
	Shader flatShader("shader/default.vert", "shader/gui.frag");
	Shader guiShader("shader/gui.vert", "shader/gui.frag");
	Shader fontShader("shader/gui.vert", "shader/font.frag");
	glm::mat4 guiProjection = glm::ortho(0.0f, (float)width, (float)width, 0.0f, -1.0f, 1.0f);	
	guiShader.setProjection(guiProjection);
	fontShader.setProjection(guiProjection);

	Log::log(TAG, "shaders initialized");

	AssetTexture planksDiffuse = ImageTexture("assets/planks.png", TextureType::Diffuse);
	AssetTexture planksSpecular = ImageTexture("assets/planks.png", TextureType::Specular, GL_UNSIGNED_BYTE, true);
	AssetTexture metalDiffuse = ImageTexture("assets/metal.jpg", TextureType::Diffuse);
	AssetTexture metalSpecular = ImageTexture("assets/metal.jpg", TextureType::Specular, GL_UNSIGNED_BYTE, true);
	AssetTexture stoneDiffuse = ImageTexture("assets/stone.jpg", TextureType::Diffuse);
	AssetTexture stoneSpecular = ImageTexture("assets/stone.jpg", TextureType::Specular, GL_UNSIGNED_BYTE, true);
	FontTexture fontTex("assets/pixel_operator_short_dollar.ttf");

	Log::log(TAG, "textures initialized");

	Globals::Font = &fontTex;
	Globals::DefaultShader = &shader;
	Globals::LightShader = &lightShader;
	Globals::FlatShader = &flatShader;
	Globals::GuiShader = &guiShader;
	Globals::FontShader = &fontShader;

	// start making 3d objects
	std::vector<Shape3D*> objects;

	// make debug pyramid
	// DebugPyramid pyramid(&planksDiffuse, &planksSpecular, glm::vec3(0.0f, 0.0f, 0.0f));
	// objects.push_back(&pyramid);

	// make sphere
	Sphere sphere(&planksDiffuse, &planksSpecular, glm::vec3(2.0f, 0.0f, 0.0f), 0.2f);
	objects.push_back(&sphere);

	// make rectangular prism
	RectangularPrism rect(&metalDiffuse, &metalSpecular, glm::vec3(-2.0f, 0.0f, 0.0f), 0.5f, 1.0f, 0.75f);
	rect.setRotation(0, 0, glm::radians(180.0f));
	objects.push_back(&rect);

	// make floor
	RectangularPrism floor(&stoneDiffuse, &stoneSpecular, glm::vec3(0.0f, -1.2f, 0.0f), 5.0f, 0.1f, 5.0f);
	objects.push_back(&floor);

	RectangularPrism rightWall(&stoneDiffuse, &stoneSpecular, glm::vec3(2.5f, 1.3f, 0.0f), 0.1f, 5.0f, 5.0f);
	RectangularPrism leftWall(&stoneDiffuse, &stoneSpecular, glm::vec3(-2.5f, 1.3f, 0.0f), 0.1f, 5.0f, 5.0f);
	// RectangularPrism backWall(&stoneDiffuse, &stoneSpecular, glm::vec3(0.0f, 1.3f, 2.5f), 5.0f, 5.0f, 0.1f);
	// RectangularPrism frontWall(&stoneDiffuse, &stoneSpecular, glm::vec3(0.0f, 1.3f, -2.5f), 5.0f, 5.0f, 0.1f);

	objects.push_back(&rightWall);
	objects.push_back(&leftWall);
	// objects.push_back(&backWall);
	// objects.push_back(&frontWall);

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

	LightController lc(window);
	lc.registerShapes(objects);
	lc.registerDrawable(&player);
	lc.processLighting();

	Log::log(TAG, "initial lighting processing completed");

	// make text
	Text playerDebugText;
	playerDebugText.setBounds(10, 10, 400, 200);
	playerDebugText.setFontSize(20);
	playerDebugText.setCenterText(false);

	Window w(window);
	w.registerListener(&lc);

	SettingsController sc;
	SettingsMenu settingsMenu(&sc);
	settingsMenu.setCorners(100, 100, width - 100, height - 100);
	sc.registerListeners({&settingsMenu, &lc, &player, &w});
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
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	Log::log(TAG, fmt::format("configuring viewport: 0, 0, {}, {}", fbWidth, fbHeight));

	glViewport(0, 0, fbWidth, fbHeight);
	glEnable(GL_CULL_FACE); // enable back face culling

	Log::log(TAG, "everything is set up; starting main game loop");

	while (!glfwWindowShouldClose(window)) {
		w.startFrame();

		player.handleKeyInputs(window, w.deltaTime);

		glEnable(GL_DEPTH_TEST); // enable depth buffer so that stuff in front blocks stuff behind it
		lc.render(*player.getActiveCamera(), w.deltaTime);
		glDisable(GL_DEPTH_TEST); // disable for gui drawing

		playerDebugText.setText(player.getDebugString());
		playerDebugText.draw();
		button.draw();

		if (settingsMenu.isOpen) settingsMenu.draw();

		glfwPollEvents();
		// end frame here since glfwSwapBuffers() suspends when using vsync
		w.endFrame();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}