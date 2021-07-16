//
//  Mesh.hpp
//  Neverland
//
//  Created by Admin on 13.07.2021.
//

#pragma once
#include <string>
#include <glm/glm.hpp>

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"

class Mesh {
    VertexArray va;
    IndexBuffer ib;
    VertexBuffer vb;
    unsigned int textureId;
public:
    Mesh(Vertex* vertices, unsigned int verticesCount, unsigned int* indices, unsigned int indicesCount, unsigned int textureId);
    ~Mesh();
    void draw();
};
