#pragma once

#include "../graphics/shader.h"
#include "../graphics/mesh.h"

#include <string>
#include <map>

class Shader;
class Mesh;

class ResourceManager {
public:    
    static std::string path;

    static std::map<std::string, Shader*> shaders;
    static std::map<const char*, _voxels*> rowmodels;

    static void init(std::string str);
    static void loadShader(std::string str);
    static void loadTexture();
    static void loadModel();

    static void addShader(Shader* shader, std::string name);
	static void addRowModel(_voxels* row, const char* name);
	static void addMesh(Mesh* mesh);

    static Shader* getShader(std::string name);
    static _voxels* getRowModel(const char* model);
};