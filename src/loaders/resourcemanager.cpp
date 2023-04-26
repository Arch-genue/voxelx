#include "resourcemanager.h"

#include <iostream>

std::string ResourceManager::path = "";

std::map<std::string, Shader*> ResourceManager::shaders;
std::map<std::string, _voxels*> ResourceManager::rowmodels;
std::map<std::string, Texture*> ResourceManager::textures;

void ResourceManager::init(std::string str) {
    path = str;
}

void ResourceManager::loadShader(std::string str) {
    Shader* shader = load_shader(path + "shaders/" + str + ".glslv", path + "/shaders/" + str + ".glslf");
	if (shader == nullptr) {
        std::cerr << "Failed to load shader: " << str << "\n";
        return;
    }
    addShader(shader, str);
}
void ResourceManager::loadTexture(std::string str) {
	Texture* texture = load_texture(path + "textures/" + str + ".png");
	if (texture == nullptr) {
        std::cerr << "Failed to load texture: " << str << "\n";
        return;
    }
    addTexture(texture, str.c_str());
}
void ResourceManager::loadModel(std::string str, std::string type) {
    _voxels* voxs;
    if (type == "voxtxt") {
        voxs = load_model(path + "models/" + str + ".voxtxt", type.c_str());
        if (voxs == nullptr) {
            std::cerr << "Failed to load model: " << str << "\n";
            delete voxs;
            return;
        }
    } else if(type == "null") voxs = genVoxel();
	addModel(voxs, str);
}

void ResourceManager::addShader(Shader* shader, std::string name) {
	shaders[name] = shader;
}
void ResourceManager::addTexture(Texture* texture, std::string name) {
	textures[name] = texture;
}
void ResourceManager::addModel(_voxels* row, std::string name) {
	rowmodels[name] = row;
}

Shader* ResourceManager::getShader(std::string name) {
	return shaders[name];
}
Texture* ResourceManager::getTexture(std::string name) {
	return textures[name];
}
_voxels* ResourceManager::getModel(std::string model) {
	return rowmodels[model];
}