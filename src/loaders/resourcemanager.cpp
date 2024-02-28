#include "resourcemanager.h"
#include "../utils.h"

#include <iostream>

std::string ResourceManager::_path = "";

std::map<std::string, Shader*> ResourceManager::_shaders;
std::map<std::string, VoxelModel*> ResourceManager::_rowmodels;
std::map<std::string, Texture*> ResourceManager::_textures;
std::map<std::string, Particles*> ResourceManager::_particles;

std::map<std::string, FT_Face> ResourceManager::_faces;

void ResourceManager::init(std::string path) {
    _path = path;
    errorprint("RESMGR", "ResourceManager initialized",  MSGINFO);
}

void ResourceManager::cleanup() {
    float time;

    errorprint("RESMGR", "Deleting shaders...",  MSGINFO);
    time = measureFunctionTime(deleteShaders);
    errorprint("RESMGR", "Shaders deleted. Time: " + BLUE_COLOR_STR + std::to_string(time) + "s" + RESET_COLOR_STR,  MSGINFO);

    errorprint("RESMGR", "Deleting models...",  MSGINFO);
    time = measureFunctionTime(deleteModels);
    errorprint("RESMGR", "Models deleted. Time: " + BLUE_COLOR_STR + std::to_string(time) + "s" + RESET_COLOR_STR,  MSGINFO);

    errorprint("RESMGR", "Deleting textures...",  MSGINFO);
    time = measureFunctionTime(deleteTextures);
    errorprint("RESMGR", "Textuers deleted. Time: " + BLUE_COLOR_STR + std::to_string(time) + "s" + RESET_COLOR_STR,  MSGINFO);

    errorprint("RESMGR", "Deleting particle systems...",  MSGINFO);
    time = measureFunctionTime(deleteParticles);
    errorprint("RESMGR", "Particle systems deleted. Time: " + BLUE_COLOR_STR + std::to_string(time) + "s" + RESET_COLOR_STR,  MSGINFO);
}

void ResourceManager::deleteShaders() {
    for (auto it = _shaders.begin(); it != _shaders.end(); it++) {
        delete it->second;
    }
    _shaders.clear();
}

void ResourceManager::deleteModels() {
    for (auto it = _rowmodels.begin(); it != _rowmodels.end(); it++) {
        delete it->second;
    }
    _rowmodels.clear();
}

void ResourceManager::deleteTextures() {
    for (auto it = _textures.begin(); it != _textures.end(); it++) {
        delete it->second;
    }
    _textures.clear();
}

void ResourceManager::deleteParticles() {
    for (auto it = _particles.begin(); it != _particles.end(); it++) {
        delete it->second;
    }
    _particles.clear();
}

void ResourceManager::loadShader(std::string str) {
    Shader* shader = load_shader(_path + "shaders/" + str + ".glslv", _path + "/shaders/" + str + ".glslf");
	if (shader == nullptr) {
        errorprint("RESMGR", "Failed to load shader: " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR),  MSGERROR);
        std::exit(1);
        return;
    }
    addShader(shader, str);
    errorprint("RESMGR", "Shader loaded:  " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR),  MSGSUCCESS);
}
void ResourceManager::loadTexture(std::string str) {
	Texture* texture = load_texture(_path + "textures/" + str + ".png");
	if (texture == nullptr) {
        errorprint("RESMGR", "Failed to load texture: " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR),  MSGERROR);
        std::exit(1);
        return;
    }
    addTexture(texture, str);
    errorprint("RESMGR", "Texture loaded: " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR),  MSGSUCCESS);
}
void ResourceManager::loadModel(std::string str, std::string type) {
    VoxelModel* voxels;
    if (type == "voxtxt") {
        voxels = load_model(_path + "models/" + str + ".voxtxt", type.c_str());
        if (voxels == nullptr) {
            errorprint("RESMGR", "Failed to load model: " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR),  MSGERROR);
            delete voxels;
            std::exit(1);
            return;
        }
    } else if(type == "null") voxels = genVoxel();
	addModel(voxels, str);
    errorprint("RESMGR", "Model loaded: " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR),  MSGSUCCESS);
}
void ResourceManager::loadVoxelParticles(std::string str) {
	Particles* particles = VoxelParticles::load_voxel_particles(_path + "particles/" + str + ".voxpart");
	if (particles == nullptr) {
        errorprint("RESMGR", "Failed to load voxel particles: " + std::string(CYAN_COLOR) + str,  MSGERROR);
        std::exit(1);
        return;
    }
    addParticles(particles, str);
    errorprint("RESMGR", "Particles System loaded: " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR),  MSGSUCCESS);
}

void ResourceManager::loadFont(std::string str) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    FT_Face face;
    std::string strfull = "../res/fonts/" + str + ".ttf";
    if (FT_New_Face(ft, strfull.c_str(), 0, &face)) {
        errorprint("RESMGR", "Failed to load font: " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR),  MSGERROR);
        std::exit(1);
        return;
    }
    FT_Set_Pixel_Sizes(face, 0, 48);
    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

    errorprint("RESMGR", "Font loaded: " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR),  MSGSUCCESS);
    _faces[str] = face;
}

void ResourceManager::prepareModel(std::string str) {
    Mesh* mesh = Renderer::render(_rowmodels[str]);
    _rowmodels[str]->setMesh(mesh);
}

void ResourceManager::addShader(Shader* shader, std::string name) {
	_shaders[name] = shader;
}
void ResourceManager::addTexture(Texture* texture, std::string name) {
	_textures[name] = texture;
}
void ResourceManager::addModel(VoxelModel* row, std::string name) {
    row->setName(name);
	_rowmodels[name] = row;
}
void ResourceManager::addParticles(Particles* particles, std::string name) {
	_particles[name] = particles;
}
// void ResourceManager::addFont(Font* particles, std::string name) {
// 	_particles[name] = particles;
// }

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
FT_Face ResourceManager::getFont(std::string name) {
    return _faces[name];
}
