#include "resourcemanager.h"

#include <filesystem>
#include <fstream>
#include <iostream>

#include "../utilities/vtype.h"
#include "../utilities/logger.h"

std::string ResourceManager::_path = "";

std::map<std::string, Shader*> ResourceManager::_shaders;
std::map<std::string, VoxelModel*> ResourceManager::_rowmodels;
std::map<std::string, Texture*> ResourceManager::_textures;

std::map<std::string, FT_Face> ResourceManager::_faces;

FT_Library ResourceManager:: _ft;

void ResourceManager::init(std::string path) {
    _path = path;

    if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
        vLogger::eprint("RESMGR", "Resources not found", LOGLEVEL::ERROR);
        std::exit(1);
        return;
    }

    if (FT_Init_FreeType(&_ft)) {
        vLogger::eprint("RESMGR", "Failed to init FreeType Library",  LOGLEVEL::ERROR);
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

    vLogger::eprint("RESMGR", "Deleting textures...",  LOGLEVEL::INFO);
    time = measureFunctionTime(deleteTextures);
    vLogger::eprint("RESMGR", "Textures deleted. Time: " + BLUE_COLOR_STR + std::to_string(time) + "s" + RESET_COLOR_STR,  LOGLEVEL::INFO);
}

void ResourceManager::deleteShaders() {
    for (auto it = _shaders.begin(); it != _shaders.end(); it++) {
        delete it->second;
    }
    _shaders.clear();
}

void ResourceManager::deleteModels() {
    // for (auto it = _rowmodels.begin(); it != _rowmodels.end(); it++) {
    //     delete it->second;
    // }
    _rowmodels.clear();
}

void ResourceManager::deleteTextures() {
    for (auto it = _textures.begin(); it != _textures.end(); it++) {
        delete it->second;
    }
    _textures.clear();
}

void ResourceManager::loadShader(std::string str) {
    Shader* shader = load_shader(_path + "shaders/" + str + ".glslv", _path + "/shaders/" + str + ".glslf");
	if (shader == nullptr) {
        vLogger::eprint("RESMGR", "Failed to load shader: " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR),  LOGLEVEL::ERROR);
        std::exit(1);
        return;
    }
    addShader(shader, str);
    vLogger::eprint("RESMGR", "Shader loaded:  " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR),  LOGLEVEL::SUCCESS);
}
void ResourceManager::loadTexture(std::string str) {
	Texture* texture = load_texture(_path + "textures/" + str + ".png");
	if (texture == nullptr) {
        vLogger::eprint("RESMGR", "Failed to load texture: " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR),  LOGLEVEL::ERROR);
        std::exit(1);
        return;
    }
    addTexture(texture, str);
    vLogger::eprint("RESMGR", "Texture loaded: " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR),  LOGLEVEL::SUCCESS);
}
void ResourceManager::loadModel(std::string str, std::string type) {
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

VoxelModel* ResourceManager::load_model(std::string filename, const char* type) {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream in(filename);
    int max = std::numeric_limits<int>::max();
    int min = std::numeric_limits<int>::min();
    if (in.is_open()) {
        std::string line;

        int min_x = max; int min_y = max; int min_z = max;
        int max_x = min; int max_y = min; int max_z = min;
        size_t vi = 0;

        float clr_r = 0.0f, clr_g = 0.0f, clr_b = 0.0f, clr_a = 0.0f;

        std::string str[10];

        uint voxelcount = 0;

        VoxelModel* voxmodel = new VoxelModel();

        while (getline(in, line)) {
            vi++;
            if ( vi < 5 ) continue;
            split(str, line, ' '); // x y z clr

            int x = atoi(str[0].c_str());
            int y = atoi(str[1].c_str());
            int z = atoi(str[2].c_str());

            min_x = (x < min_x) ? x : min_x;
            min_y = (y < min_y) ? y : min_y;
            min_z = (z < min_z) ? z : min_z;

            max_x = (x > max_x) ? x : max_x;
            max_y = (y > max_y) ? y : max_y;
            max_z = (z > max_z) ? z : max_z;
            
            //* VOXEL COLOR
            std::string hex = str[3];
            int r = std::stoi(hex.substr(0, 2), nullptr, 16);
            int g = std::stoi(hex.substr(2, 2), nullptr, 16);
            int b = std::stoi(hex.substr(4, 2), nullptr, 16);
            clr_r = r / 255.0f;
            clr_g = g / 255.0f;
            clr_b = b / 255.0f;
            clr_a = 1.0f;
            
            Voxel vox;
            vox.position = glm::ivec3(x, y, z);
            vox.color = glm::vec4(clr_r, clr_g, clr_b, clr_a);
            
            voxmodel->setVoxel(x, y, z, vox);
        }

        if ( min_x == max ) min_x = 0;
        if ( min_y == max ) min_y = 0;
        if ( min_z == max ) min_z = 0;
        if ( max_x == min ) max_x = 0;
        if ( max_y == min ) max_y = 0;
        if ( max_z == min ) max_z = 0;

        std::cout << min_x << " " << min_y << " " << min_z << "\n";
        std::cout << max_x << " " << max_y << " " << max_z << "\n";

        voxmodel->setMinSize(glm::vec3(min_x, min_y, min_z));
        voxmodel->setSize(glm::vec3(max_x + 1, max_y + 1, max_z + 1));

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
    VoxelModel* nullvox = new VoxelModel(glm::ivec3(1));
    Voxel voxel;
    voxel.color = glm::vec4(0, 0, 0, 0);
    voxel.visible = false;

    nullvox->setVoxel(0, 0, 0, voxel);
    return nullvox;
}

void ResourceManager::loadFont(std::string str) {
    FT_Face face;
    std::string strfull = _path + "fonts/" + str + ".ttf";
    if (FT_New_Face(_ft, strfull.c_str(), 0, &face)) {
        vLogger::eprint("RESMGR", "Failed to load font: " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR), LOGLEVEL::ERROR);
        std::exit(1);
        return;
    }
    FT_Set_Pixel_Sizes(face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

    vLogger::eprint("RESMGR", "Font loaded: " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR), LOGLEVEL::SUCCESS);
    _faces[str] = face;
}

void ResourceManager::prepareModel(const std::string& str) {
    std::unique_ptr<Mesh> mesh = Renderer::render(_rowmodels[str]);
    _rowmodels[str]->setMesh(std::move(mesh));
    vLogger::eprint("RESMGR", "Model prepared: " + std::string(CYAN_COLOR) + str + std::string(RESET_COLOR), LOGLEVEL::SUCCESS);
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

Shader* ResourceManager::getShader(std::string name) {
	return _shaders[name];
}
Texture* ResourceManager::getTexture(std::string name) {
	return _textures[name];
}
VoxelModel* ResourceManager::getModel(std::string name) {
    auto it = _rowmodels.find(name);
    if (it == _rowmodels.end() || it->second == nullptr) {
        vLogger::eprint("RESMGR", "Model not found: " + name, LOGLEVEL::ERROR);
        std::exit(1);
    }

	return it->second;
}

FT_Face ResourceManager::getFont(std::string name) {
    return _faces[name];
}
