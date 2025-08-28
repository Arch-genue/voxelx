#include "resourcemanager.h"

#include <filesystem>
#include <fstream>
#include <iostream>

#include "../utilities/vtype.h"
#include "../utilities/logger.h"

std::string ResourceManager::_path = "";

std::map<std::string, Shader*> ResourceManager::_shaders;
std::map<std::string, VoxelModel*> ResourceManager::_voxelmodels;
std::map<std::string, MeshModel*> ResourceManager::_meshmodels;

void ResourceManager::init(std::string path) {
    _path = path;

    if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
        vLogger::eprint("RESMGR", "Resources not found", LOGLEVEL::ERROR);
        std::exit(1);
        return;
    }

    vLogger::eprint("RESMGR", "ResourceManager initialized",  LOGLEVEL::INFO);
}

void ResourceManager::cleanup() {
    float time;

    vLogger::eprint("RESMGR", "Deleting shaders...",  LOGLEVEL::INFO);
    time = measureFunctionTime(deleteShaders);
    vLogger::eprint("RESMGR", "Shaders deleted. Time: " + BLUE_COLOR_STR + std::to_string(time) + "s" + RESET_COLOR_STR,  LOGLEVEL::INFO);

    vLogger::eprint("RESMGR", "Deleting models...",  LOGLEVEL::INFO);
    time = measureFunctionTime(deleteModels);
    vLogger::eprint("RESMGR", "Models deleted. Time: " + BLUE_COLOR_STR + std::to_string(time) + "s" + RESET_COLOR_STR,  LOGLEVEL::INFO);
}

void ResourceManager::deleteShaders() {
    for (auto it = _shaders.begin(); it != _shaders.end(); it++) {
        delete it->second;
    }
    _shaders.clear();
}

void ResourceManager::deleteModels() {
    for (auto it = _voxelmodels.begin(); it != _voxelmodels.end(); it++) {
        delete it->second;
    }
    _voxelmodels.clear();
}

void ResourceManager::loadShader(const std::string& str) {
    Shader* shader = load_shader(_path + "shaders/" + str + ".glslv", _path + "/shaders/" + str + ".glslf");
	if (shader == nullptr) {
        vLogger::eprint("RESMGR", "Failed to load shader: " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR),  LOGLEVEL::ERROR);
        std::exit(1);
        return;
    }
    addShader(shader, str);
    vLogger::eprint("RESMGR", "Shader loaded:  " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR),  LOGLEVEL::SUCCESS);
}

void ResourceManager::loadModel(const std::string& str, const std::string& type) {
    VoxelModel* voxels;
    if (type == "voxtxt") {
        voxels = load_model(_path + "models/" + str + ".voxtxt", type.c_str());
        if (voxels == nullptr) {
            vLogger::eprint("RESMGR", "Failed to load model: " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR),  LOGLEVEL::ERROR);
            std::exit(1);
            return;
        }
    } else if(type == "null") {
        voxels = genVoxel();
    }
	addModel(voxels, str);
    vLogger::eprint("RESMGR", "Model loaded: " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR),  LOGLEVEL::SUCCESS);
}

VoxelModel* ResourceManager::load_model(const std::string& filename, const char* type) {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream in(filename);
    if (in.is_open()) {
        std::string line;

        float clr_r = 0.0f, clr_g = 0.0f, clr_b = 0.0f, clr_a = 0.0f;
        
        VoxelModel* voxmodel = new VoxelModel();
        
        size_t vi = 0;
        while (getline(in, line)) {
            vi++;
            if ( vi < 5 ) continue;
            auto parts = split(line, ' '); // x y z clr
            if (parts.size() < 4) continue;

            int x = std::stoi(parts[0]);
            int y = std::stoi(parts[1]);
            int z = std::stoi(parts[2]);
            
            //* VOXEL COLOR
            std::string hex = parts[3];
            clr_r = std::stoi(hex.substr(0, 2), nullptr, 16) / 255.0f;
            clr_g = std::stoi(hex.substr(2, 2), nullptr, 16) / 255.0f;
            clr_b = std::stoi(hex.substr(4, 2), nullptr, 16) / 255.0f;
            clr_a = 1.0f;

            Voxel& voxel = voxmodel->getVoxel(x, y, z);
            voxel.setColor(glm::vec4(clr_r, clr_g, clr_b, clr_a));
            voxel.visible = true;
        }

        in.close();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = end - start;
        vLogger::eprint("LOADER", "Loaded model: " + std::string(CYAN_COLOR) + filename + "	" + std::string(BLUE_COLOR) + std::to_string(duration.count()) + "s" + std::string(RESET_COLOR),  LOGLEVEL::INFO);
        return voxmodel;
    } else {
        return nullptr;
    }
}

VoxelModel* ResourceManager::genVoxel() {
    VoxelModel* nullvox = new VoxelModel();

    Voxel& voxel = nullvox->getVoxel(0, 0, 0);
    voxel.setColor(glm::vec4(0, 0, 0, 0));
    voxel.visible = true;
        
    return nullvox;
}

void ResourceManager::prepareModel(const std::string& str) {
    auto start = std::chrono::high_resolution_clock::now();

    MeshModel* meshmodel = Renderer::generateMeshModel(_voxelmodels[str]);
    _meshmodels[str] = meshmodel; 

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end - start;

    vLogger::eprint("RESMGR", "Model prepared: " + std::string(CYAN_COLOR) + str + "	" + std::string(BLUE_COLOR) + std::to_string(duration.count()) + "s" + std::string(RESET_COLOR), LOGLEVEL::SUCCESS);
}

void ResourceManager::addShader(Shader* shader, const std::string& name) {
	_shaders[name] = shader;
}

void ResourceManager::addModel(VoxelModel* row, const std::string& name) {
    row->setName(name);
	_voxelmodels[name] = row;
}

void ResourceManager::loadShaders() {
    std::string folder_path = _path + "shaders/";
    vtype::fndvector<std::string> files;
    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
        if (entry.is_regular_file()) {
            std::string name = entry.path().filename();
            auto splitstr = split(name, '.'); // x y z clr
            // if (parts.size() < 4) continue;
            if (!files.contains(splitstr[0])) {
                files.push_back(splitstr[0]);
                loadShader(splitstr[0]);
            }
        }
    }
}

void ResourceManager::loadModels() {
    std::string folder_path = _path + "models/";
    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
        if (entry.is_regular_file()) {
            std::string name = entry.path().filename();
            auto splitstr = split(name, '.'); // x y z clr
            loadModel(splitstr[0], "voxtxt");
            ResourceManager::prepareModel(splitstr[0]);
        }
    }
}

Shader* ResourceManager::getShader(const std::string& name) {
	return _shaders[name];
}

VoxelModel* ResourceManager::getModel(const std::string& name) {
    auto it = _voxelmodels.find(name);
    if (it == _voxelmodels.end() || it->second == nullptr) {
        vLogger::eprint("RESMGR", "Model not found: " + name, LOGLEVEL::ERROR);
        std::exit(1);
    }

	return it->second;
}

MeshModel* ResourceManager::getMeshFromModel(const std::string& name) {
    return _meshmodels[name];
}