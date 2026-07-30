#include"TextRenderer.h"
#include"mesh/Mesh.h"
#include"util/Log.h"

TextRenderer::TextRenderer(int windowWidth, int windowHeight)
    : windowWidth(windowWidth), windowHeight(windowHeight),
      fontTex("assets/pixel_operator_short_dollar.ttf"),
      fontShader("shader/gui.vert", "shader/font.frag")
{
	glm::mat4 guiProjection = glm::ortho(0.0f, (float)windowWidth, (float)windowHeight, 0.0f, -1.0f, 1.0f);	
	fontShader.setProjection(guiProjection);
}

void TextRenderer::drawText(const std::string& text, int x, int y, int lineHeight, int maxWidth, glm::vec3 textColor) {
    std::vector<Vertex> vertices = fontTex.generateVertices(text, x, y, lineHeight, maxWidth);
    std::vector<GLuint> indices = fontTex.generateIndices(vertices);

    Mesh mesh(vertices, indices, {&fontTex});
    fontShader.setTextColor(textColor);
    mesh.drawGui(fontShader);
}