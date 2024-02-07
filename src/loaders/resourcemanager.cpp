#include "resourcemanager.h"

#include <iostream>

std::string ResourceManager::_path = "";

std::map<std::string, Shader*> ResourceManager::_shaders;
std::map<std::string, VoxelModel*> ResourceManager::_rowmodels;
std::map<std::string, Texture*> ResourceManager::_textures;
std::map<std::string, Particles*> ResourceManager::_particles;

void ResourceManager::init(std::string path) {
    _path = path;
}

void ResourceManager::loadShader(std::string str) {
    Shader* shader = load_shader(_path + "shaders/" + str + ".glslv", _path + "/shaders/" + str + ".glslf");
	if (shader == nullptr) {
        std::cerr << "Failed to load shader: " << str << "\n";
        return;
    }
    addShader(shader, str);
    std::cout << "Shader loaded: " << str << std::endl;
}
void ResourceManager::loadTexture(std::string str) {
	Texture* texture = load_texture(_path + "textures/" + str + ".png");
	if (texture == nullptr) {
        std::cerr << "Failed to load texture: " << str << "\n";
        return;
    }
    addTexture(texture, str);
    std::cout << "Texture loaded: " << str << std::endl;
}
void ResourceManager::loadModel(std::string str, std::string type) {
    VoxelModel* voxels;
    if (type == "voxtxt") {
        voxels = load_model(_path + "models/" + str + ".voxtxt", type.c_str());
        if (voxels == nullptr) {
            std::cerr << "Failed to load model: " << str << "\n";
            delete voxels;
            return;
        }
    } else if(type == "null") voxels = genVoxel();
	addModel(voxels, str);
    std::cout << "Model loaded: " << str << std::endl;
}
void ResourceManager::loadVoxelParticles(std::string str) {
	Particles* particles = VoxelParticles::load_voxel_particles(_path + "particles/" + str + ".voxpart");
	if (particles == nullptr) {
        std::cerr << "Failed to load voxel particles: " << str << "\n";
        return;
    }
    addParticles(particles, str);
    std::cout << "Particles loaded: " << str << std::endl;
}

void ResourceManager::addShader(Shader* shader, std::string name) {
	_shaders[name] = shader;
}
void ResourceManager::addTexture(Texture* texture, std::string name) {
	_textures[name] = texture;
}
void ResourceManager::addModel(VoxelModel* row, std::string name) {
	_rowmodels[name] = row;
}
void ResourceManager::addParticles(Particles* particles, std::string name) {
	_particles[name] = particles;
}

Shader* ResourceManager::getShader(std::string name) {
	return _shaders[name];
}
Texture* ResourceManager::getTexture(std::string name) {
	return _textures[name];
}
VoxelModel* ResourceManager::getModel(std::string name) {
	return _rowmodels[name];
}
Particles* ResourceManager::getParticles(std::string name) {
    return _particles[name];
}