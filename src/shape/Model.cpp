#include"Model.h"
#include "texture/ImageTexture.h"
#include "util/Log.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

Model::Model(std::string path) {
    loadModel(path);

    float scale = 1.0f / 200.0f;
    glm::vec3 scaleFactors = glm::vec3(scale, scale, scale); 
    model = glm::scale(model, scaleFactors);
}

void Model::loadModel(std::string path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);	
	
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        Log::err(TAG, import.GetErrorString());
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    // process all the node's meshes (if any)
    for (uint i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        meshes.push_back(processMesh(mesh, scene));			
    }
    // then do the same for each of its children
    for (uint i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture*> textures;

    // walk through each of the mesh's vertices
    for (uint i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector; 
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;
        // normals
        if (mesh->HasNormals()) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texUV = vec;
        }        
        // colours
        if (mesh->mColors[0]) {
            vertex.color = {
                mesh->mColors[0][i].r,
                mesh->mColors[0][i].g,
                mesh->mColors[0][i].b
            };
            Log::log(TAG, fmt::format("vertex.color = {}, {}, {}", vertex.color.r, vertex.color.g, vertex.color.b));
        }

        vertices.push_back(vertex);
    }
    // now walk through each of the mesh's faces and retrieve the corresponding vertex indices.
    for (uint i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (uint j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);        
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    

    // diffuse maps
    std::vector<Texture*> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // specular maps
    std::vector<Texture*> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    if (textures.empty()) {
        // it must be using colours instead of textures
        glm::vec3 color = glm::vec3(1.0f);
        aiColor3D aiColor;
        if (material->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor) == AI_SUCCESS) {
            color = {aiColor.r, aiColor.g, aiColor.b};
            Log::log(TAG, fmt::format("color: {}, {}, {}", color.r, color.g, color.b));
        } else {
            Log::warn(TAG, "processMesh() found a mesh without textures or material colours!");
        }
        return Mesh(vertices, indices, color);
    } else {
        return Mesh(vertices, indices, textures);
    }
}

std::vector<Texture*> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type) {
    std::vector<Texture*> textures;
    // TODO: weird stuff might happen if there's more than one texture per type, since shader only supports that
    for (uint i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        auto texture = std::make_unique<ImageTexture>(
            (directory + std::string(str.C_Str())).c_str(),
            aiToTextureType(type)
        );
        // texture.path = str;
        textures.push_back(texture.get());
        this->textures.push_back(std::move(texture));
    }
    return textures;
}

TextureType Model::aiToTextureType(aiTextureType type) {
    switch (type) {
        case aiTextureType_DIFFUSE: return TextureType::Diffuse;
        case aiTextureType_SPECULAR: return TextureType::Specular;
        default:
            Log::err(TAG, fmt::format("unrecognized texture type! {}", (int) type));
            return TextureType::Diffuse;
    }
}

void Model::draw(Camera& camera, Shader& shader) {
    shader.setModel(model);
    shader.setShininess(16); // TODO
    for (auto& mesh: meshes) {
        mesh.draw(camera, shader);
    }
}

void Model::drawToDepthMap(PointLightCamera& camera, Shader& depthShader) {
    depthShader.setModel(model);
    for (auto& mesh: meshes) {
        mesh.drawToDepthMap(camera, depthShader);
    }
}