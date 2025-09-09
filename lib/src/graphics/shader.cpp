#include "shader.h"

#include <exception>
#include <iostream>
#include <sstream>
#include <fstream>

#include <glm/gtc/type_ptr.hpp>

#include "utilities/logger.hpp"

Shader::Shader(const std::filesystem::path& path) {
    std::string t = split(std::string(path), '.')[0];
    // Logger::instance().log(LogLevel::DEBUG, "Test", t + ".glslv", " ", t + ".glslf");

    _id = this->load_shader(t + ".glslv", t + ".glslf");
    if (_id == -1) {
        Logger::instance().log(LogLevel::ERROR, "Shader", "Shader compile error brcyan<", std::string(path.filename()), ">");
        std::exit(2);
    }
}
Shader::~Shader() {
    if (_id != 0) glDeleteProgram(_id);
}

Shader& Shader::use() {
    glUseProgram(_id);
    return *this;
}

static GLint getLoc(GLuint prog, const std::string& name) {
    GLint loc = glGetUniformLocation(prog, name.c_str());
    if (loc == -1) {
        // Logger::instance().log("SHADER", "Uniform not found: " + name, LogLevel::WARNING);
    }
    return loc;
}

void Shader::uniformMatrix(const std::string &name, const glm::mat4& matrix) {
    GLint loc = getLoc(_id, name);
    if (loc != -1) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}
void Shader::uniformFloat(const std::string &name, float val) {
    GLint loc = getLoc(_id, name);
    if (loc != -1) glUniform1f(loc, val);
}

void Shader::uniformVec3(const std::string &name, const glm::vec3& var) {
    GLint loc = getLoc(_id, name);
    if (loc != -1) glUniform3f(loc, var.x, var.y, var.z);
}

void Shader::uniformLight(const std::string &name, const Light &light) {
    uniformVec3(name + ".position", light.position);
    uniformVec3(name + ".direction", light.direction);
    uniformFloat(name + ".cutOff", light.cutOff);
    uniformFloat(name + ".outerCutOff", light.outerCutOff);

    // light properties
    uniformVec3(name + ".ambient", light.ambient);
    uniformVec3(name + ".diffuse", light.diffuse);
    uniformVec3(name + ".specular", light.specular);
    uniformFloat(name + ".constant", light.constant);
    uniformFloat(name + ".linear", light.linear);
    uniformFloat(name + ".quadratic", light.quadratic);
}

void Shader::uniformMaterial(const std::string& name, const Material &material) {
    uniformVec3(name + ".ambient", material.ambient);
    uniformVec3(name + ".diffuse", material.diffuse);
    uniformVec3(name + ".specular", material.specular);
    uniformFloat(name + ".shininess", material.shininess);
}

GLuint Shader::compileShader(const GLchar* code, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        throw std::runtime_error(std::string("Shader compilation failed: ") + infoLog);
    }
    return shader;
}

GLuint Shader::load_shader(const std::string& vertexFile, const std::string& fragmentFile) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vShaderFile.open(vertexFile);
        std::stringstream vShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        vShaderFile.close();

        vertexCode = vShaderStream.str();
    } catch (std::ifstream::failure& e) {
        Logger::instance().log(LogLevel::ERROR, "SHADER", "Could not read fragment vertex file brcyan<", vertexFile ,">");
        return -1;
    }
    try {        
        fShaderFile.open(fragmentFile);
        std::stringstream fShaderStream;

        fShaderStream << fShaderFile.rdbuf();

        fShaderFile.close();

        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure& e) {
        Logger::instance().log(LogLevel::ERROR, "SHADER", "Could not read fragment shader file brcyan<", fragmentFile ,">");
        return -1;
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
        Logger::instance().log(LogLevel::ERROR, "SHADER", "VERTEX SHADER COMPILATION FAILED: brred<", log, ">");
        glDeleteShader(vertex);
        return -1;
    }

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::string log = infoLog;
        Logger::instance().log(LogLevel::ERROR, "SHADER", "FRAGMENT SHADER COMPILATION FAILED: brred<", log, ">");
        glDeleteShader(vertex);
        return -1;
    }

    // Shader Program
    GLuint _id = glCreateProgram();
    glAttachShader(_id, vertex);
    glAttachShader(_id, fragment);
    glLinkProgram(_id);

    glGetProgramiv(_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_id, 512, nullptr, infoLog);
        Logger::instance().log(LogLevel::ERROR, "SHADER", "LINKING_FAILED: brred<", infoLog, ">");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
        glDeleteProgram(_id);
        return -1;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return _id;
}