#include "Shape3D.h"
#include "util/Log.h"

Shape3D::Shape3D(glm::vec3 position, bool isLightSource) 
    : position(position),
      isLightSource(isLightSource),
      shader(
        (isLightSource) ? "shader/light.vert" : "shader/default.vert", 
        (isLightSource) ? "shader/light.frag" : "shader/default.frag"
      ) {}

Shape3D::Shape3D(Texture* diffuse, Texture* specular, glm::vec3 position, bool isLightSource) 
    : Shape3D(position, isLightSource)
{
    std::vector<Texture*> textures;
    if (diffuse != nullptr) {
        textures = {diffuse, specular};
    }
    mesh.setMaterial(Material{&shader, textures});
    model = glm::translate(model, position);
    configureShader(model);
}

void Shape3D::invalidateShape() {
    vertices = generateVertices();
    indices = generateIndices();

    glm::mat4 rotation(1.0f);

    if (rotationX) rotation = glm::rotate(rotation, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
    if (rotationY) rotation = glm::rotate(rotation, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
    if (rotationZ) rotation = glm::rotate(rotation, glm::radians(rotationZ), glm::vec3(0.0f, 0.0f, 1.0f));

    for (auto& vertex: vertices) {
        vertex.normal = glm::vec3(rotation * glm::vec4(vertex.normal, 1.0f));
    }

    mesh.setShapeData(        
        vertices,
        indices
    );
}

void Shape3D::invalidateModel() {
    model = glm::translate(glm::mat4(1.0f), position); // reset model; apply translation first and rotation second since matrix multiplication in glm works from right to left
     
    if (rotationX) model = glm::rotate(model, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
    if (rotationY) model = glm::rotate(model, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
    if (rotationZ) model = glm::rotate(model, glm::radians(rotationZ), glm::vec3(0.0f, 0.0f, 1.0f));

    configureShader(model);
}

void Shape3D::setRotation(float rotationX, float rotationY, float rotationZ) {
    this->rotationX = rotationX;
    this->rotationY = rotationY;
    this->rotationZ = rotationZ;
    
    invalidateShape();
    invalidateModel();
}
    
void Shape3D::setColor(glm::vec3 color, float intensity) {
    shader.activate();

    if (isLightSource) {
        if (intensity < 0) Log::warn(TAG, fmt::format("setColor() given light range {} is less than 0!", intensity));
        glUniform3f(glGetUniformLocation(shader.ID, "color"), color.x, color.y, color.z);
        light.color = color;
        light.range = intensity;
    } else {
        if (intensity < 0 || intensity > 1) Log::warn(TAG, fmt::format("setColor() given colour tint intensity {} is not between 0 and 1!", intensity));
        glUniform3f(glGetUniformLocation(shader.ID, "tintColor"), color.x, color.y, color.z);
        glUniform1f(glGetUniformLocation(shader.ID, "tintIntensity"), intensity);
    }
}

void Shape3D::configureShader(glm::mat4 model) {
	shader.activate();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

void Shape3D::registerLightSource(int num, glm::vec3 lightColor, glm::vec3 lightPos, float linear, float quadratic) {
    Log::log(TAG, fmt::format("registerLightSource() num = {} linear = {}, quadratic = {}", num, linear, quadratic));
    shader.activate();

    std::string pointLightUniform = "pointLights[0]";
    pointLightUniform[pointLightUniform.size() - 2] = num + '0';
	glUniform3f(glGetUniformLocation(shader.ID, (pointLightUniform + ".color").c_str()), lightColor.x, lightColor.y, lightColor.z);
	glUniform3f(glGetUniformLocation(shader.ID, (pointLightUniform + ".position").c_str()), lightPos.x, lightPos.y, lightPos.z);
	glUniform1f(glGetUniformLocation(shader.ID, (pointLightUniform + ".constant").c_str()), 1.0);
	glUniform1f(glGetUniformLocation(shader.ID, (pointLightUniform + ".linear").c_str()), linear);
	glUniform1f(glGetUniformLocation(shader.ID, (pointLightUniform + ".quadratic").c_str()), quadratic);
}

void Shape3D::setNumPointLights(int num) {
    shader.activate();
	glUniform1i(glGetUniformLocation(shader.ID, "numPointLights"), num);
}

void Shape3D::draw(Camera& camera) {
    mesh.draw(camera);
}