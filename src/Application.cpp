#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/gtc/matrix_transform.hpp>

#include "Shader.hpp"
#include "Mesh.hpp"

void processInput(GLFWwindow* window);
void saveTextureToVRAM(const char* path, unsigned int* textureId);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

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
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    unsigned int grassTextureId;
    saveTextureToVRAM("grass1.png", &grassTextureId);

    Vertex vertices[] = {
        Vertex( glm::vec3( 0.5f,  0.5f, 0.0f),  glm::vec2( 1.0f, 1.0f) ),
        Vertex( glm::vec3( 0.5f, -0.5f, 0.0f),  glm::vec2( 1.0f, 0.0f) ),
        Vertex( glm::vec3(-0.5f, -0.5f, 0.0f),  glm::vec2( 0.0f, 0.0f) ),
        Vertex( glm::vec3(-0.5f,  0.5f, 0.0f),  glm::vec2( 0.0f, 1.0f) )
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    {
        Shader base = Shader("Vertex.glsl", "Fragment.glsl");
        Mesh mesh = Mesh(vertices, 4, indices, 6, grassTextureId);
        
        glm::mat4 projectionMatrix;
        // projectionMatrix = glm::perspective(90.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
        projectionMatrix = glm::mat4(1.0f);
        
        float time;
        while (!glfwWindowShouldClose(window))
        {
            time = glfwGetTime();
            processInput(window);

            glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            base.use();
            
            glm::mat4 transform = glm::mat4(1.0f);
            // transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 1.0f));
            // transform = glm::rotate(transform, time, glm::vec3(0.0f, 1.0f, 0.0f));
            base.setMat4("transform", transform);
            base.setMat4("projection", projectionMatrix);
            
            mesh.draw();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void saveTextureToVRAM(const char* path, unsigned int* textureId) {
    glGenTextures(1, textureId);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, *textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "Failed to load texture at path " << path << std::endl;
    }
    stbi_image_free(data);
}
