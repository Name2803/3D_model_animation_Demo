#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderCl.h"

#include <string>


struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class BFMesh
{
public:
    unsigned int VAO;

    BFMesh(Vertex* vertices_, Texture* textures_, unsigned int* indeces_, int* counts_);
    ~BFMesh();

    void draw(Shader shader);
    unsigned int EBO, VBO;
    Vertex* vertices;
    Texture* textures;
    unsigned int* indeces;
    int* counts;
private:
    void setup_mesh();

};