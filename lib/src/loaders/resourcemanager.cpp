#include "resourcemanager.h"

#include <filesystem>
#include <fstream>
#include <iostream>

#include "../utilities/vtype.hpp"
#include "../utilities/logger.hpp"

std::string ResourceManager::_path = "";

std::map<std::string, Shader*> ResourceManager::_shaders;
std::map<std::string, VoxelModel*> ResourceManager::_voxelmodels;
std::map<std::string, MeshModel*> ResourceManager::_meshmodels;
vtype::fndvector<std::string> ResourceManager::_scripts;

void ResourceManager::init(std::string path) {
    _path = path;

    if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
        Logger::instance().log(LogLevel::ERROR, "RESMGR", "Resources not found");
        std::exit(1);
        return;
    }

    Logger::instance().log(LogLevel::INFO, "RESMGR", "ResourceManager initialized");
}

void ResourceManager::cleanup() {
    float time;

    Logger::instance().log(LogLevel::INFO, "RESMGR", "Deleting shaders...");
    time = measureFunctionTime(deleteShaders);
    Logger::instance().log(LogLevel::INFO, "RESMGR", "Shaders deleted. Time: brmagenta<", std::to_string(time), "s>");

    Logger::instance().log(LogLevel::INFO, "RESMGR", "Deleting models...");
    time = measureFunctionTime(deleteModels);
    Logger::instance().log(LogLevel::INFO, "RESMGR", "Models deleted. Time: brmagenta<", std::to_string(time), "s>");
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
        Logger::instance().log(LogLevel::ERROR, "RESMGR", "Failed to load shader brcyan<", str, ">");
        std::exit(1);
        return;
    }
    addShader(shader, str);
    Logger::instance().log(LogLevel::SUCCESS, "RESMGR", "Shader loaded brcyan<", str, ">");
}

void ResourceManager::loadModel(const std::string& str, const std::string& type) {
    auto start = std::chrono::high_resolution_clock::now();
    VoxelModel* voxels;
    if (type == "voxtxt") {
        voxels = load_model(_path + "models/" + str + ".voxtxt", type.c_str());
        if (voxels == nullptr) {
            Logger::instance().log(LogLevel::ERROR, "RESMGR", "Failed to load model brcyan<", str, ">");
            std::exit(1);
            return;
        }
    } else if(type == "null") {
        voxels = genVoxel();
    }
	addModel(voxels, str);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end - start;
    Logger::instance().log(LogLevel::SUCCESS, "RESMGR", "Model loaded  brcyan<", str, ">   brmagenta<", std::to_string(duration.count()), "s>");

    // Logger::instance().log(LogLevel::SUCCESS, "RESMGR", "Model loaded brcyan<", str, ">");
}

VoxelModel* ResourceManager::load_model(const std::string& filename, const char* type) {
    // auto start = std::chrono::high_resolution_clock::now();

    std::ifstream in(filename);
    if (in.is_open()) {
        std::string line;

        float clr_r = 0.0f, clr_g = 0.0f, clr_b = 0.0f, clr_a = 0.0f;
        
        VoxelModel* voxmodel = new VoxelModel();
        
        size_t vi = 0;
        while (getline(in, line)) {
            if ( ++vi < 5 ) continue;
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
        // auto end = std::chrono::high_resolution_clock::now();
        // std::chrono::duration<float> duration = end - start;
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

    Logger::instance().log(LogLevel::SUCCESS, "RESMGR", "Model prepared: brcyan<", str, ">   brmagenta<", std::to_string(duration.count()), "s>");
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

void ResourceManager::loadScripts() {
    std::string folder_path = "../scripts/";
    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
        if (entry.is_regular_file()) {
            std::string name = entry.path().filename();
            name = split(name, '.')[0];
            bool t = ScriptSystem::instance().loadScript(entry.path());
            if (t) {
                _scripts.push_back(name);
            } else {
                Logger::instance().log(LogLevel::ERROR, "ResManager", "Failed to load script brcyan<", name, ">");
            }
        }
    }
}

Shader* ResourceManager::getShader(const std::string& name) {
	return _shaders[name];
}

VoxelModel* ResourceManager::getModel(const std::string& name) {
    auto it = _voxelmodels.find(name);
    if (it == _voxelmodels.end() || it->second == nullptr) {
        Logger::instance().log(LogLevel::ERROR, "RESMGR", "Model not found: brcyan<", name, ">");
        std::exit(1);
    }

	return it->second;
}

MeshModel* ResourceManager::getMeshFromModel(const std::string& name) {
    return _meshmodels[name];
}