//
//  Renderer.hpp
//  Neverland
//
//  Created by Admin on 16.07.2021.
//

#pragma once
#include <iostream>
#include <glad/glad.h>

static void GLClearError() {
    while(glGetError() != GL_NO_ERROR);
}

static void GLCheckError() {
    while(GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
    }
}
