#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Shader.hpp"
#include "Mesh.hpp"

void processInput(GLFWwindow* window);

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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Neverland", NULL, NULL);
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
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(NULL);

    Vertex vertices[] = {
        Vertex( glm::vec3(100.f, 100.f, -10.0f),  glm::vec2( 1.0f, 1.0f) ),
        Vertex( glm::vec3(100.f, -100.f, -10.0f),  glm::vec2( 1.0f, 0.0f) ),
        Vertex( glm::vec3(-100.f, -100.f, -10.0f),  glm::vec2( 0.0f, 0.0f) ),
        Vertex( glm::vec3(-100.f, 100.f, -10.0f),  glm::vec2( 0.0f, 1.0f) )
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    {
        Shader base = Shader("Vertex.glsl", "Fragment.glsl");
        Texture grass1("grass1.png");
        std::vector<Texture*> textures;
        textures.push_back(&grass1);
        Mesh mesh = Mesh(vertices, 4, indices, 6, textures);
        
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(400.f, 400.f, 0.f));
        // glm::mat4 projectionMatrix = glm::perspective(90.0f, 1.0f, 0.1f, 1000.0f);
        glm::mat4 projectionMatrix = glm::ortho(0.0f, 800.0f, 0.0f, 800.0f, 0.f, 1.0f);
        glm::mat4 model = glm::mat4(1.f);
        
        base.use();
        base.setMat4("projection", projectionMatrix);
        
        // model - transform of model
        // view - transform of camera
        // projection - transform to NDC
    
        glm::vec4 clearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glm::vec3 translate(0.f);
        
        float time;
        while (!glfwWindowShouldClose(window))
        {
            time = glfwGetTime();
            processInput(window);

            glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            base.use();
            base.setInt("texture1", 0);
            model = glm::scale(glm::mat4(1.f), glm::vec3(abs(sin(time)) + 1.f, abs(sin(time)) + 1.f, 0.f));
            model = glm::rotate(model, time, glm::vec3(0.f, 0.f, 1.f));
            model = glm::translate(model, translate);
            base.setMat4("view", view);
            base.setMat4("model", model);
            
            mesh.draw();
            
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            
            ImGui::Begin("Neverland begins...");
            ImGui::Text("Translation of land:");
            ImGui::SliderFloat2("Translate", &translate.x, -300.f, 300.f);
            ImGui::ColorEdit3("Clear color:", (float*)&clearColor); // Edit 3 floats representing a color
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,ImGui::GetIO().Framerate);
            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
