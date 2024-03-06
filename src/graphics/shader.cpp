#include "shader.h"

#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

#include "../utilities/logger.h"

Shader::Shader(unsigned int id) : id(id) {}

Shader::~Shader() {
    glDeleteProgram(id);
}

void Shader::use() {
    glUseProgram(id);
}

void Shader::uniformMatrix(std::string name, glm::mat4 matrix) {
    GLuint transformLoc = glGetUniformLocation(id, name.c_str());
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}
void Shader::uniformFloat(std::string name, float val) {
    GLuint transformLoc = glGetUniformLocation(id, name.c_str());
    glUniform1f(transformLoc, val);
}

void Shader::uniformVec3(std::string name, glm::vec3 var) {
    GLuint transformLoc = glGetUniformLocation(id, name.c_str());
    glUniform3f(transformLoc, var.x, var.y, var.z);
}

void Shader::uniformLight(std::string name, Light &light) {
    uniformVec3("light.position", light.position);
    uniformVec3("light.direction", light.direction);
    uniformFloat("light.cutOff", light.cutOff);
    uniformFloat("light.outerCutOff", light.outerCutOff);

    // light properties
    uniformVec3("light.ambient", light.ambient);
    // we configure the diffuse intensity slightly higher; the right lighting conditions differ with each lighting method and environment.
    // each environment and lighting type requires some tweaking to get the best out of your environment.
    uniformVec3("light.diffuse", light.diffuse);
    uniformVec3("light.specular", light.specular);
    uniformFloat("light.constant", light.constant);
    uniformFloat("light.linear", light.linear);
    uniformFloat("light.quadratic", light.quadratic);
}

void Shader::uniformMaterial(std::string name, Material &material) {
    uniformVec3(name + ".ambient", material.ambient);
    uniformVec3(name + ".diffuse", material.diffuse);
    uniformVec3(name + ".specular", material.specular);
    uniformFloat(name + ".shininess", material.shininess);
}

Shader * load_shader(std::string vertexFile, std::string fragmentFile) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::badbit);
    try {
        vShaderFile.open(vertexFile);
        fShaderFile.open(fragmentFile);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure& e) {
        Logger::eprint("SHADER", "FILE_NOT_SUCCESSFULLY_READ",  LOGLEVEL::ERROR);
        return nullptr;
    }
    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();

    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::string log = infoLog;
        Logger::eprint("SHADER", "VERTEX SHADER COMPILATION FAILED: " + RED_COLOR_STR + log + RESET_COLOR_STR,  LOGLEVEL::ERROR);
        return nullptr;
    }

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::string log = infoLog;
        Logger::eprint("SHADER", "FRAGMENT SHADER COMPILATION FAILED: " + RED_COLOR_STR + log + RESET_COLOR_STR,  LOGLEVEL::ERROR);
        return nullptr;
    }

    // Shader Program
    GLuint id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::LINKING_FAILED\n";
        std::cerr << infoLog << std::endl;

        glDeleteShader(vertex);
        glDeleteShader(fragment);
        glDeleteProgram(id);
        return nullptr;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return new Shader(id);
}