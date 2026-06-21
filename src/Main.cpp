#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Mesh.h"
#include"FontTexture.h"
#include"ImageTexture.h"
#include"Gui.h"
#include"Log.h"

const unsigned int width = 800;
const unsigned int height = 800;
const std::string TAG = "Main";

// Vertices coordinates
Vertex vertices[] =
{ //                      COORDINATES           /        NORMALS          /            COLOURS          /  TEXTURE COORDINATES  //
	Vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(0.0f, 0.0f)},  // Bottom side
	Vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(0.0f, 5.0f)},  // Bottom side
	Vertex{glm::vec3( 0.5f, 0.0f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(5.0f, 5.0f)},  // Bottom side
	Vertex{glm::vec3( 0.5f, 0.0f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(5.0f, 0.0f)},  // Bottom side

	Vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(-0.8f, 0.5f, 0.0f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(0.0f, 0.0f)},  // Left Side
	Vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(-0.8f, 0.5f, 0.0f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(5.0f, 0.0f)},  // Left Side
	Vertex{glm::vec3( 0.0f, 0.8f,  0.0f), glm::vec3(-0.8f, 0.5f, 0.0f), glm::vec3(0.92f, 0.86f, 0.76f), glm::vec2(2.5f, 5.0f)},  // Left Side

	Vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 0.5f, -0.8f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(5.0f, 0.0f)},  // Non-facing side
	Vertex{glm::vec3( 0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 0.5f, -0.8f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(0.0f, 0.0f)},  // Non-facing side
	Vertex{glm::vec3( 0.0f, 0.8f,  0.0f), glm::vec3(0.0f, 0.5f, -0.8f), glm::vec3(0.92f, 0.86f, 0.76f), glm::vec2(2.5f, 5.0f)},  // Non-facing side

	Vertex{glm::vec3( 0.5f, 0.0f, -0.5f), glm::vec3(0.8f, 0.5f,  0.0f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(0.0f, 0.0f)},  // Right side
	Vertex{glm::vec3( 0.5f, 0.0f,  0.5f), glm::vec3(0.8f, 0.5f,  0.0f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(5.0f, 0.0f)},  // Right side
	Vertex{glm::vec3( 0.0f, 0.8f,  0.0f), glm::vec3(0.8f, 0.5f,  0.0f), glm::vec3(0.92f, 0.86f, 0.76f), glm::vec2(2.5f, 5.0f)},  // Right side

	Vertex{glm::vec3( 0.5f, 0.0f,  0.5f), glm::vec3(0.0f, 0.5f,  0.8f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(5.0f, 0.0f)},  // Facing side
	Vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(0.0f, 0.5f,  0.8f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(0.0f, 0.0f)},  // Facing side
	Vertex{glm::vec3( 0.0f, 0.8f,  0.0f), glm::vec3(0.0f, 0.5f,  0.8f), glm::vec3(0.92f, 0.86f, 0.76f), glm::vec2(2.5f, 5.0f)},  // Facing side
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2, // Bottom side
	0, 2, 3, // Bottom side
	4, 6, 5, // Left side
	7, 9, 8, // Non-facing side
	10, 12, 11, // Right side
	13, 15, 14 // Facing side
};

Vertex lightVertices[] =
{ //               COORDINATES            // normal        // color        // texture       //
	Vertex{glm::vec3(-0.1f, -0.1f,  0.1f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)},
	Vertex{glm::vec3(-0.1f, -0.1f, -0.1f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)},
	Vertex{glm::vec3( 0.1f, -0.1f, -0.1f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)},
	Vertex{glm::vec3( 0.1f, -0.1f,  0.1f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)},
	Vertex{glm::vec3(-0.1f,  0.1f,  0.1f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)},
	Vertex{glm::vec3(-0.1f,  0.1f, -0.1f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)},
	Vertex{glm::vec3( 0.1f,  0.1f, -0.1f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)},
	Vertex{glm::vec3( 0.1f,  0.1f,  0.1f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)}
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1, 1, 5, 4, 1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

// square
Vertex guiVertices[] =
{
	Vertex{glm::vec3(200.0f, 600.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(200.0f, 200.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3(600.0f, 200.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(600.0f, 600.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)}
};

// square
GLuint guiIndices[] =
{
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

	Log::log(TAG, Log::oss("configuring viewport: 0, 0, ", fbWidth, ", ", fbHeight));

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

	// make light cube mesh
	Shader lightShader("shader/light.vert", "shader/light.frag");
	Material lightMaterial { &lightShader, {} };

	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	Mesh light(lightVerts, lightInd, lightMaterial);

	// make gui
	FontTexture guiDiffuse = FontTexture("assets/PixelOperator.ttf");
	Gui gui(&guiDiffuse, width, height);

	// make models - shader uses model to place vertices around correct location in the world
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);

	// configure shaders
	lightShader.activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetKeyCallback(window, keyCallback);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // background colour
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the back buffer

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		camera.handleKeyInputs(window, deltaTime);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		pyramid.draw(camera);
		light.draw(camera);
		gui.drawText(camera.getDebugString(), 200, 200, 400, 20);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
