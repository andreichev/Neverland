#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Shader.h"

void processInput(GLFWwindow* window);
void createVertexArray(unsigned int bufferId, unsigned int* vertexArrayId);
void saveTriangleCoordsToVRAM(unsigned int* bufferId);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
};

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    Shader base = Shader("Vertex.glsl", "Fragment.glsl");

    unsigned int bufferId;
    saveTriangleCoordsToVRAM(&bufferId);

    unsigned int vertexArrayId;
    createVertexArray(bufferId, &vertexArrayId);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        base.use();
        glBindVertexArray(vertexArrayId);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glUseProgram(0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vertexArrayId);
    glDeleteBuffers(1, &bufferId);
    base.deleteProgram();

    glfwTerminate();
    return 0;
}

void saveTriangleCoordsToVRAM(unsigned int* bufferId) {
    glGenBuffers(1, bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, *bufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void createVertexArray(unsigned int bufferId, unsigned int* vertexArrayId) {
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glGenVertexArrays(1, vertexArrayId);
    glBindVertexArray(*vertexArrayId);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*) 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
