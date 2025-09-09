#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <filesystem>

#include "graphics/shader.h"
#include "graphics/meshmodel.h"
#include "gamesystems/gamemodel.h"
#include "scripting/scriptsystem.hpp"
#include "utilities/utils.h"

#include <iostream>

/**
 * @brief Менеджер ресурсов v2.0
 * 
 */
class AssetManager {
private:
    AssetManager() = default;
    ~AssetManager() = default;
    AssetManager(const AssetManager&) = delete; // оператор копирования
    AssetManager& operator=(const AssetManager&) = delete; // оператор присваивания

    template <typename T>
    std::unordered_map<std::string, std::unique_ptr<T>>& getContainer() {
        static std::unordered_map<std::string, std::unique_ptr<T>> container;
        return container;
    }

    std::string _path;
    std::vector<std::string> _scripts;
public:
    // Singleton
    static AssetManager& instance() {
        static AssetManager inst;
        return inst;
    }

    void init(std::string path);
    
    void loadShaders();
    void loadGameModels();

    template <typename T>
    T &create(const std::filesystem::path &path) {
        auto asset = std::make_unique<T>(path);
        std::string name = split(path.filename(), '.')[0];
        T& ref = *asset;
        // Logger::instance().log(LogLevel::ERROR, "T", path, " ", name);
        auto& container = this->getContainer<T>();
        container[name] = std::move(asset);
        return ref;
    }

    MeshModel &create(VoxelStructure& structure) {
        std::string name = structure.getName() + ":meshmodel";
        auto asset = std::make_unique<MeshModel>(&structure);
        MeshModel& ref = *asset;
        auto& container = this->getContainer<MeshModel>();
        container[name] = std::move(asset);
        return ref;
    }

    template <typename T>
    std::unique_ptr<T> clone(const std::string& name) {
        auto& container = getContainer<T>();
        auto it = container.find(name);
        if (it != container.end() && it->second) {
            return std::make_unique<T>(*(it->second));
        }
        return nullptr;
    }
    
    template <typename T>
    T* get(const std::string& name) {
        auto& container = getContainer<T>();
        auto it = container.find(name);
        return (it != container.end()) ? it->second.get() : nullptr;
    }
};