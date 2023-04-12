#pragma once

#include <string>
#include "../graphics/shader.h"
#include "../graphics/mesh.h"

class Shader;
class Mesh;

class ResourceLoader {
public:    
    static std::string respath;
    static Shader* shaders;
    static Mesh* meshes;
    static void setPath(std::string str);
    static void loadShaders();
    static void loadTextures();
    static void loadModels();
};