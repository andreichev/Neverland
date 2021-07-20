//
//  Renderer.hpp
//  Neverland
//
//  Created by Admin on 16.07.2021.
//

#pragma once
#include <iostream>
#include <glad/glad.h>

#define GLCall(x) GLClearError();\
    x;\
    if(GLLogCall(#x, __FILE__, __LINE__) == false) {\
        assert(false);\
    }

static void GLClearError() {
    while(glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line) {
    if(GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << ") " << function << " " << file << " :" << line << std::endl;
        return false;
    }
    return true;
}
