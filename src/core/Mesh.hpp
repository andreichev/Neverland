//
//  Mesh.hpp
//  Neverland
//
//  Created by Admin on 13.07.2021.
//

#pragma once
#include <vector>
#include <string>

#include <glm/glm.hpp>

struct Vertex {
    // position
    glm::vec3 Position;
    // texCoords
    glm::vec2 TexCoords;
    
    Vertex(glm::vec3 aPosition, glm::vec2 aTexCoords): Position(aPosition), TexCoords(aTexCoords) {};
};

class Mesh {
    unsigned int textureId, vertexBufferId, indexBufferId, vertexArrayId;
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int textureId);
    void draw();
    void unload();
};
