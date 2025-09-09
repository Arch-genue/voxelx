#include "assetmanager.h"

#include "utilities/logger.hpp"

void AssetManager::init(std::string path) {
    _path = path;

    if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
        Logger::instance().log(LogLevel::ERROR, "Asset Manager", "Assets path is not exists");
        std::exit(1);
    }

    Logger::instance().log(LogLevel::INFO, "RESMGR", "Asset Manager initialized: brcyan<", path, ">");
}

void AssetManager::loadShaders() {
    std::string folder_path = _path + "shaders/";
    std::vector<std::string> files;
    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
        if (entry.is_regular_file()) {
            std::string name = split(std::string(entry.path().filename()), '.')[0];
            if (std::find(files.begin(), files.end(), name) == files.end()) {
                files.push_back(name);
                this->create<Shader>(entry.path());
            }
        }
    }
}

void AssetManager::loadGameModels() {
    std::string folder_path = _path + "gamemodels/";
    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
        this->create<GameModel>(entry.path());
    }
}