#pragma once

#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <filesystem>

#include "types/material.h"

class Shader {
public:
    Shader(const std::filesystem::path& path);
    ~Shader();

    Shader& use();
    void uniformMatrix(const std::string& name, const glm::mat4& matrix);
    void uniformFloat(const std::string& name, float val);
    void uniformVec3(const std::string& name, const glm::vec3& var);
    
    //* Special functions */
    void uniformLight(const std::string& name, const Light &light);
    void uniformMaterial(const std::string& name, const Material &material);    
private:
    GLuint _id;
    std::string _path;

    GLuint compileShader(const GLchar *code, GLenum type);
    GLuint load_shader(const std::string& vertexFile, const std::string& fragmentFile);
};
