#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "core/Shader.hpp"
#include "Mesh.hpp"

void processInput(GLFWwindow* window);
void saveTextureToVRAM(const char* path, unsigned int* textureId);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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

    unsigned int grassTextureId;
    saveTextureToVRAM("grass1.png", &grassTextureId);

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    vertices.push_back( Vertex( glm::vec3( 0.5f,  0.5f, 0.0f),  glm::vec2( 1.0f, 1.0f) ) ); // top right
    vertices.push_back( Vertex( glm::vec3( 0.5f, -0.5f, 0.0f),  glm::vec2( 1.0f, 0.0f) ) ); // bottom right
    vertices.push_back( Vertex( glm::vec3(-0.5f, -0.5f, 0.0f),  glm::vec2( 0.0f, 0.0f) ) ); // bottom left
    vertices.push_back( Vertex( glm::vec3(-0.5f,  0.5f, 0.0f),  glm::vec2( 0.0f, 1.0f) ) ); // top left
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(3);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(3);
    
    Shader base = Shader("Vertex.glsl", "Fragment.glsl");
    Mesh mesh = Mesh(vertices, indices, grassTextureId);
    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        base.use();
        mesh.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    mesh.unload();
    base.deleteProgram();

    glfwTerminate();
    return 0;
}

void saveTextureToVRAM(const char* path, unsigned int* textureId) {
    glGenTextures(1, textureId);
    glBindTexture(GL_TEXTURE_2D, *textureId); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
