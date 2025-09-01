#pragma once

#include "graphics/renderer.h"
#include "graphics/shader.h"
#include "scripting/scriptsystem.hpp"

#include <string>
#include <map>

class Shader;
class MeshModel;

/**
 * @brief Менеджер ресурсов
 * 
 */
class ResourceManager {
    static std::string _path;

    static std::map<std::string, Shader*> _shaders;
    static std::map<std::string, VoxelModel*> _voxelmodels;
    static std::map<std::string, MeshModel*> _meshmodels;
    static vtype::fndvector<std::string> _scripts;

public:
    static void init(std::string str);
    static void cleanup();

    static void deleteShaders();
    static void deleteModels();

    static void loadShaders();
    static void loadModels();
    static void loadScripts();

    static void loadShader(const std::string& str);
    static void loadModel(const std::string& str, const std::string& type);

    static void prepareModel(const std::string& str);

    static void addShader(Shader* shader, const std::string& name);
	static void addModel(VoxelModel* row, const std::string& name);
	static void addMesh(MeshModel* mesh);

    static Shader* getShader(const std::string& name);
    static VoxelModel* getModel(const std::string& name);
    static MeshModel* getMeshFromModel(const std::string& name);

    static VoxelModel* load_model(const std::string& filename, const char *type);
    static VoxelModel* genVoxel();
};