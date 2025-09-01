/**
 * @file mesh.h
 * @author Vlad Kartsaev
 * @brief Mesh class implementation
 * @version 0.5
 * @date 2023-04-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <unordered_map>
#include "voxels/voxelmodel.hpp"
#include "mesh.h"

/**
 * @brief Класс MeshModel, который хранит меши чанков модели VoxelModel
 * Теперь является основной моделью, которая уже взаимодействует с ModelObject
 */
class MeshModel {
public:
    MeshModel(VoxelModel* voxelmodel): _voxelmodel(voxelmodel) {};
    VoxelModel* getVoxelModel() { return _voxelmodel; }

    void draw(unsigned int primitive, glm::mat4 modelmatrix, Shader* shader) {
        this->forEachMesh([&](VoxelModel::ChunkType& chunk, Mesh* mesh) {
            mesh->draw(primitive, modelmatrix, shader);
        });
    }

    template<typename Func>
    void forEachMesh(Func func) {
        for (auto& [chunk, mesh] : _map) {
            func(*chunk, mesh);
        }
    }

    void set(VoxelModel::ChunkTypePtr chunk, Mesh* mesh) {
        _map[chunk] = mesh;
    }

    Mesh* get(VoxelModel::ChunkTypePtr chunk) const {
        auto it = _map.find(chunk);
        return (it != _map.end()) ? it->second : nullptr;
    }

    void remove(VoxelModel::ChunkTypePtr chunk) {
        auto it = _map.find(chunk);
        if (it != _map.end()) {
            delete it->second;   // если _map владеет Mesh
            _map.erase(it);
        }
    }

private:
    std::unordered_map<VoxelModel::ChunkTypePtr, Mesh*> _map;
    VoxelModel* _voxelmodel;
};