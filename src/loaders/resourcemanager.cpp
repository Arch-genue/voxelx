#include "resourcemanager.h"

#include <iostream>

std::string ResourceManager::path = "";
std::map<std::string, Shader*> ResourceManager::shaders;
std::map<const char*, _voxels*> ResourceManager::rowmodels;

void ResourceManager::init(std::string str) {
    path = str;
}

void ResourceManager::loadShader(std::string str) {
    Shader* shader = load_shader(path + "shaders/" + str + ".glslv", path + "/shaders/" + str + ".glslf");
    addShader(shader, str.c_str());
}

void ResourceManager::addShader(Shader* shader, std::string name) {
	shaders[name] = shader;
}
void ResourceManager::addRowModel(_voxels* row, const char* name) {
	rowmodels[name] = row;
}
// void ResourceManager::addMesh(Mesh* mesh) {
// 	meshes[_meshindex] = mesh;
// 	_meshindex++;
// }

Shader* ResourceManager::getShader(std::string name) {
	return shaders[name];
}

_voxels* ResourceManager::getRowModel(const char* model) {
	return rowmodels[model];
}