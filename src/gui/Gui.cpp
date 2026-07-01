#include "Gui.h"
#include"util/Log.h"
#include"mesh/Mesh.h"

Gui::Gui(FontTexture* t, int windowW, int windowH)
    : fontTex(t),
      shader("shader/gui.vert", "shader/font.frag") 
{
    configureShader(windowW, windowH);
}

Gui::Gui(ImageTexture* t, int windowW, int windowH)
    : imageTex(t),
      shader("shader/gui.vert", "shader/gui.frag")
{
    configureShader(windowW, windowH);
}

void Gui::draw(int x, int y, int w, int h) {
    // TODO
}

void Gui::drawText(const std::string& text, int x, int y, int w, int lineHeight) {
    if (fontTex == nullptr) {
        Log::err(TAG, "drawText() called when fontTex is null! not drawing anything.");
        return;
    }

    std::vector<Vertex> vertices = fontTex->generateVertices(text, x, y, w, lineHeight);
    std::vector<GLuint> indices = fontTex->generateIndices(vertices);
	std::vector<Texture*> textures;
	textures.push_back(fontTex);

    Material m = Material {&shader, textures};
	Mesh mesh(vertices, indices, m);
    mesh.drawGui();
}

void Gui::configureShader(int windowW, int windowH) {
    shader.activate();
    glm::mat4 guiProjection = glm::ortho(0.0f, (float)windowW, (float)windowH, 0.0f, -1.0f, 1.0f);	
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(guiProjection));
}