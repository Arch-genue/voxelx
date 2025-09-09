#include "gamemodel.h"

#include "vendor/json.hpp"
#include "utilities/logger.hpp"
#include "assetmanager/assetmanager.h"

#include <fstream>
#include <iostream>
#include <string>

using json = nlohmann::json;

GameModel::GameModel(const std::filesystem::path& path): _path(path), _editable(true) {
    int state = this->load_gamemodel(path);
    if (state != 0) {
        Logger::instance().log(LogLevel::ERROR, "GameModel", "GameModel load error brcyan<", std::string(path.filename()), ">");
        std::exit(2);
    }
}

GameModel::GameModel(const GameModel &other): _editable(other._editable), _gamemodelinfo(other._gamemodelinfo)
//_texture(other._texture), _collision(other._collision)
{
    // if(other._structure)
    _structure = AssetManager::instance().clone<VoxelStructure>(other.getStructure()->getName()).get();
    // _meshmodel = AssetManager::instance().clone<MeshModel>();

    std::cout << "_STRUCT " << _structure << "\n";
}

uint8_t GameModel::load_gamemodel(const std::string& path) {
    std::string meta = path + "/meta.json"; // Мета файл с информацией о модели
    if (!std::filesystem::exists(meta)) {
        return -1;
    }
    std::ifstream file(meta);
    if (!file.is_open()) {
        std::cerr << "Cannot open JSON file\n";
        return -2;
    }

    // парсим JSON
    json j;
    file >> j;

    // читаем поля
    std::string modelName = j["name"];
    std::string modelFile = j["model"]["file"];
    std::string modelType = j["model"]["type"];
    std::string shaderFragment;
    std::string shaderVertex;

    if (j.contains("shader")) {
        shaderFragment = j["shader"]["fragment"];
        shaderVertex = j["shader"]["vertex"];
    } else {
        shaderFragment = "voxelx:default";
        shaderVertex = "voxelx:default";
    }

    auto pivot = j["pivot"];
    float pivotX = pivot[0];
    float pivotY = pivot[1];
    float pivotZ = pivot[2];

    // std::string diffuse = j["textures"]["diffuse"];
    // std::string normal = j["textures"]["normal"];

    // std::string collisionType = j["collision"]["type"];
    // float collisionSize = j["collision"]["size"];

    std::cout << "Name: " << modelName << ", Model: " << modelFile << "\n";
    // ! Shaders
    // TODO

    // ! Structure
    _structure = &AssetManager::instance().create<VoxelStructure>(std::filesystem::path(path + "/" + modelFile));
    _model = &AssetManager::instance().create(*_structure);

    this->_gamemodelinfo.name = modelName;
    this->_gamemodelinfo.model_file = modelFile;
    this->_gamemodelinfo.model_type = modelType;
    this->_gamemodelinfo.path = path;
    this->_gamemodelinfo.shader_fragment = shaderFragment;
    this->_gamemodelinfo.shader_vertex = shaderVertex;

    return 0;
}