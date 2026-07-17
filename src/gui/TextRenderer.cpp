#include"TextRenderer.h"
#include"mesh/Mesh.h"
#include"util/Log.h"

TextRenderer::TextRenderer(FontTexture* t): fontTex(t) {}

void TextRenderer::drawText(const std::string& text, Shader& shader, int x, int y, int w, int lineHeight, glm::vec3 textColor) {
    if (fontTex == nullptr) {
        Log::err(TAG, "drawText() called when fontTex is null! not drawing anything.");
        return;
    }

    std::vector<Vertex> vertices = fontTex->generateVertices(text, x, y, w, lineHeight);
    std::vector<GLuint> indices = fontTex->generateIndices(vertices);
	std::vector<Texture*> textures;
	textures.push_back(fontTex);

    Mesh mesh(vertices, indices, textures);
    shader.setTextColor(textColor);
    mesh.drawGui(shader);
}