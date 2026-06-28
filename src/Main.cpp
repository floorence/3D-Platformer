#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<fmt/format.h>

#include"Mesh.h"
#include"Sphere.h"
#include"RectangularPrism.h"
#include"FontTexture.h"
#include"ImageTexture.h"
#include"Gui.h"
#include"Log.h"

const unsigned int width = 800;
const unsigned int height = 800;
const std::string TAG = "Main";

// Vertices coordinates
Vertex vertices[] = {
	Vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},  // Bottom side
	Vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 5.0f)},  // Bottom side
	Vertex{glm::vec3( 0.5f, 0.0f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(5.0f, 5.0f)},  // Bottom side
	Vertex{glm::vec3( 0.5f, 0.0f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(5.0f, 0.0f)},  // Bottom side

	Vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(-0.8f, 0.5f, 0.0f), glm::vec2(0.0f, 0.0f)},  // Left Side
	Vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(-0.8f, 0.5f, 0.0f), glm::vec2(5.0f, 0.0f)},  // Left Side
	Vertex{glm::vec3( 0.0f, 0.8f,  0.0f), glm::vec3(-0.8f, 0.5f, 0.0f), glm::vec2(2.5f, 5.0f)},  // Left Side

	Vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 0.5f, -0.8f), glm::vec2(5.0f, 0.0f)},  // Non-facing side
	Vertex{glm::vec3( 0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 0.5f, -0.8f), glm::vec2(0.0f, 0.0f)},  // Non-facing side
	Vertex{glm::vec3( 0.0f, 0.8f,  0.0f), glm::vec3(0.0f, 0.5f, -0.8f), glm::vec2(2.5f, 5.0f)},  // Non-facing side

	Vertex{glm::vec3( 0.5f, 0.0f, -0.5f), glm::vec3(0.8f, 0.5f,  0.0f), glm::vec2(0.0f, 0.0f)},  // Right side
	Vertex{glm::vec3( 0.5f, 0.0f,  0.5f), glm::vec3(0.8f, 0.5f,  0.0f), glm::vec2(5.0f, 0.0f)},  // Right side
	Vertex{glm::vec3( 0.0f, 0.8f,  0.0f), glm::vec3(0.8f, 0.5f,  0.0f), glm::vec2(2.5f, 5.0f)},  // Right side

	Vertex{glm::vec3( 0.5f, 0.0f,  0.5f), glm::vec3(0.0f, 0.5f,  0.8f), glm::vec2(5.0f, 0.0f)},  // Facing side
	Vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(0.0f, 0.5f,  0.8f), glm::vec2(0.0f, 0.0f)},  // Facing side
	Vertex{glm::vec3( 0.0f, 0.8f,  0.0f), glm::vec3(0.0f, 0.5f,  0.8f), glm::vec2(2.5f, 5.0f)},  // Facing side
};

// Indices for vertices order
GLuint indices[] = {
	0, 1, 2, // Bottom side
	0, 2, 3, // Bottom side
	4, 6, 5, // Left side
	7, 9, 8, // Non-facing side
	10, 12, 11, // Right side
	13, 15, 14 // Facing side
};

// square
Vertex guiVertices[] = {
	Vertex{glm::vec3(200.0f, 200.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(200.0f, 600.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3(600.0f, 600.0f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(600.0f, 200.0f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)}
};

// square
GLuint guiIndices[] = {
	0, 2, 1,
	0, 3, 2
};

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

	// make pyramid mesh
	Texture planksDiffuse = ImageTexture("assets/planks.png", TextureType::Diffuse);
	Texture planksSpecular = ImageTexture("assets/planks.png", TextureType::Diffuse, GL_UNSIGNED_BYTE, true);
	Texture* texture_ptrs[] { &planksDiffuse, &planksSpecular };
	std::vector <Texture*> planksTextures(texture_ptrs, texture_ptrs + sizeof(texture_ptrs) / sizeof(Texture*));

	Log::log(TAG, "pyramid textures initialized");

	Shader shader("shader/default.vert", "shader/default.frag");
	Material pyramidMaterial { &shader, planksTextures };

	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	Mesh pyramid(verts, ind, pyramidMaterial);

	// make gui
	FontTexture guiDiffuse = FontTexture("assets/pixel_operator_short_dollar.ttf");
	Gui gui(&guiDiffuse, width, height);

	// make models - shader uses model to place vertices around correct location in the world
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	RectangularPrism light(nullptr, nullptr, lightPos, 0.2f, 0.2f, 0.2f, true);
	light.setColor(lightColor, 1.0f);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);

	// make sphere
	Sphere sphere(&planksDiffuse, &planksSpecular, glm::vec3(3.0f, 0.0f, 0.0f), 1);
	sphere.registerLightSource(lightColor, lightPos);

	// make rectangular prism
	Texture bunDiffuse = ImageTexture("assets/metal.jpg", TextureType::Diffuse);
	Texture bunSpecular = ImageTexture("assets/metal.jpg", TextureType::Diffuse, GL_UNSIGNED_BYTE, true);

	RectangularPrism rect(&bunDiffuse, &bunSpecular, glm::vec3(-3.0f, 0.0f, 0.0f), 0.5f, 1.0f, 0.75f);
	rect.registerLightSource(lightColor, lightPos);
	rect.setRotation(0, 0, 180);

	// configure shaders
	shader.activate();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	Log::log(TAG, "shaders initialized");

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