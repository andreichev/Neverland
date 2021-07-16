//
//  Mesh.cpp
//  Neverland
//
//  Created by Admin on 13.07.2021.
//

#include <iostream>
#include <glad/glad.h>

#include "Mesh.hpp"
#include "Renderer.hpp"

Mesh::Mesh(Vertex* vertices, unsigned int verticesCount, unsigned int* indices, unsigned int indicesCount, unsigned int textureId): ib(indices, indicesCount), vb(vertices, sizeof(Vertex) * verticesCount) {
    this->textureId = textureId;

    VertexBufferLayout layout;
    layout.pushVector();
    va.addBuffer(vb, layout);
}

Mesh::~Mesh() {}

void Mesh::draw() {
    GLCall(glBindTexture(GL_TEXTURE_2D, textureId));
    va.bind();
    ib.bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.getSize(), GL_UNSIGNED_INT, 0));
}
