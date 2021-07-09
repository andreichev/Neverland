#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void readFromFile(const char* fileName, std::string& out) {
    std::ifstream stream(fileName);
    std::stringstream stringStream;
    std::string line;
    while (getline(stream, line)) {
        stringStream << line << "\n";
    }
    out = stringStream.str();
}

void createShaderProgram(unsigned int* id) {
    *id = glCreateProgram();
    int success;
    char infoLog[512];

    std::string vertexShaderSourceString;
    readFromFile("Vertex.glsl", vertexShaderSourceString);
    const char* vertexShaderSource = vertexShaderSourceString.c_str();
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShaderId);
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    std::string fragmentShaderSourceString;
    readFromFile("Fragment.glsl", fragmentShaderSourceString);
    const char* fragmentShaderSource = fragmentShaderSourceString.c_str();
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderId, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShaderId);
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glAttachShader(*id, vertexShaderId);
    glAttachShader(*id, fragmentShaderId);

    glLinkProgram(*id);
    glGetProgramiv(*id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(*id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
}