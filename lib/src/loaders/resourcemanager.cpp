#include "resourcemanager.h"

#include <filesystem>

#include "../utilities/vtype.h"
#include "../utilities/logger.h"

std::string ResourceManager::_path = "";

std::map<std::string, Shader*> ResourceManager::_shaders;
std::map<std::string, VoxelModel*> ResourceManager::_rowmodels;
std::map<std::string, Texture*> ResourceManager::_textures;
std::map<std::string, Particles*> ResourceManager::_particles;

std::map<std::string, FT_Face> ResourceManager::_faces;

FT_Library ResourceManager:: _ft;

void ResourceManager::init(std::string path) {
    _path = path;

    if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
        Logger::eprint("RESMGR", "Resources not found", LOGLEVEL::ERROR);
        std::exit(1);
        return;
    }

    if (FT_Init_FreeType(&_ft)) {
        Logger::eprint("RESMGR", "Failed to init FreeType Library",  LOGLEVEL::ERROR);
        std::exit(1);
        return;
    }
    Logger::eprint("RESMGR", "ResourceManager initialized",  LOGLEVEL::INFO);
}

void ResourceManager::cleanup() {
    float time;

    Logger::eprint("RESMGR", "Deleting shaders...",  LOGLEVEL::INFO);
    time = measureFunctionTime(deleteShaders);
    Logger::eprint("RESMGR", "Shaders deleted. Time: " + BLUE_COLOR_STR + std::to_string(time) + "s" + RESET_COLOR_STR,  LOGLEVEL::INFO);

    Logger::eprint("RESMGR", "Deleting models...",  LOGLEVEL::INFO);
    time = measureFunctionTime(deleteModels);
    Logger::eprint("RESMGR", "Models deleted. Time: " + BLUE_COLOR_STR + std::to_string(time) + "s" + RESET_COLOR_STR,  LOGLEVEL::INFO);

    Logger::eprint("RESMGR", "Deleting textures...",  LOGLEVEL::INFO);
    time = measureFunctionTime(deleteTextures);
    Logger::eprint("RESMGR", "Textures deleted. Time: " + BLUE_COLOR_STR + std::to_string(time) + "s" + RESET_COLOR_STR,  LOGLEVEL::INFO);

    Logger::eprint("RESMGR", "Deleting particle systems...",  LOGLEVEL::INFO);
    time = measureFunctionTime(deleteParticles);
    Logger::eprint("RESMGR", "Particle systems deleted. Time: " + BLUE_COLOR_STR + std::to_string(time) + "s" + RESET_COLOR_STR,  LOGLEVEL::INFO);
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
        Logger::eprint("RESMGR", "Failed to load shader: " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR),  LOGLEVEL::ERROR);
        std::exit(1);
        return;
    }
    addShader(shader, str);
    Logger::eprint("RESMGR", "Shader loaded:  " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR),  LOGLEVEL::SUCCESS);
}
void ResourceManager::loadTexture(std::string str) {
	Texture* texture = load_texture(_path + "textures/" + str + ".png");
	if (texture == nullptr) {
        Logger::eprint("RESMGR", "Failed to load texture: " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR),  LOGLEVEL::ERROR);
        std::exit(1);
        return;
    }
    addTexture(texture, str);
    Logger::eprint("RESMGR", "Texture loaded: " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR),  LOGLEVEL::SUCCESS);
}
void ResourceManager::loadModel(std::string str, std::string type) {
    VoxelModel* voxels;
    if (type == "voxtxt") {
        voxels = load_model(_path + "models/" + str + ".voxtxt", type.c_str());
        if (voxels == nullptr) {
            Logger::eprint("RESMGR", "Failed to load model: " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR),  LOGLEVEL::ERROR);
            delete voxels;
            std::exit(1);
            return;
        }
    } else if(type == "null") voxels = genVoxel();
	addModel(voxels, str);
    Logger::eprint("RESMGR", "Model loaded: " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR),  LOGLEVEL::SUCCESS);
}
void ResourceManager::loadVoxelParticles(std::string str) {
	Particles* particles = VoxelParticles::load_voxel_particles(_path + "particles/" + str + ".voxpart");
	if (particles == nullptr) {
        Logger::eprint("RESMGR", "Failed to load voxel particles: " + std::string(CYAN_COLOR) + str,  LOGLEVEL::ERROR);
        std::exit(1);
        return;
    }
    addParticles(particles, str);
    Logger::eprint("RESMGR", "Particles System loaded: " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR),  LOGLEVEL::SUCCESS);
}

void ResourceManager::loadFont(std::string str) {
    FT_Face face;
    std::string strfull = _path + "fonts/" + str + ".ttf";
    if (FT_New_Face(_ft, strfull.c_str(), 0, &face)) {
        Logger::eprint("RESMGR", "Failed to load font: " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR),  LOGLEVEL::ERROR);
        std::exit(1);
        return;
    }
    FT_Set_Pixel_Sizes(face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

    Logger::eprint("RESMGR", "Font loaded: " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR),  LOGLEVEL::SUCCESS);
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
#include <iostream>
void ResourceManager::loadShaders() {
    std::string folder_path = _path + "shaders/";
    vtype::fndvector<std::string> files;
    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
        if (entry.is_regular_file()) {
            std::string name = entry.path().filename();
            std::string splitstr[2];
            split(splitstr, name, '.');
            if (!files.contains(splitstr[0])) {
                files.push_back(splitstr[0]);
                loadShader(splitstr[0]);
            }
        }
    }
}
void ResourceManager::loadTextures() {
    std::string folder_path = _path + "textures/";
    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
        if (entry.is_regular_file()) {
            std::string name = entry.path().filename();
            std::string splitstr[2];
            split(splitstr, name, '.');
            loadTexture(splitstr[0]);
        }
    }
}
void ResourceManager::loadModels() {
    std::string folder_path = _path + "models/";
    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
        if (entry.is_regular_file()) {
            std::string name = entry.path().filename();
            std::string splitstr[2];
            split(splitstr, name, '.');
            loadModel(splitstr[0], "voxtxt");
            ResourceManager::prepareModel(splitstr[0]);
        }
    }
}
void ResourceManager::loadParticlesSystems() {
    std::string folder_path = _path + "particles/";
    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
        if (entry.is_regular_file()) {
            std::string name = entry.path().filename();
            std::string splitstr[2];
            split(splitstr, name, '.');
            loadVoxelParticles(splitstr[0]);
        }
    }
}
void ResourceManager::loadFonts() {
    std::string folder_path = _path + "fonts/";
    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
        if (entry.is_regular_file()) {
            std::string name = entry.path().filename();
            std::string splitstr[2];
            split(splitstr, name, '.');
            loadFont(splitstr[0]);
        }
    }
}

void ResourceManager::addParticles(Particles *particles, std::string name) {
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
    if (_rowmodels[name] == nullptr) {
        Logger::eprint("RESMGR", "Model not found: " + name, LOGLEVEL::ERROR);
        std::exit(1);
        return nullptr;
    }
	return _rowmodels[name];
}
Particles* ResourceManager::getParticles(std::string name) {
    return _particles[name];
}
FT_Face ResourceManager::getFont(std::string name) {
    return _faces[name];
}
