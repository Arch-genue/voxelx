/**
 * @file meshmodel.h
 * @author Vlad Kartsaev
 * @brief Mesh container
 * @version 0.7
 * @date 2025-09-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include <unordered_map>
#include "voxels/voxelstructure.h"
#include "graphics/voxelmesh.h"
#include "graphics/renderer.h"

/**
 * @brief Класс MeshModel, который хранит меши чанков модели VoxelModel
 * Теперь является основной моделью, которая уже взаимодействует с ModelObject
 */
class MeshModel {
public:
    MeshModel(VoxelStructure* structure): _structure(structure) {
        _structure->eachChunk([&](VoxelChunk& chunk) {
            // this->set(&chunk, std::make_unique<VoxelMesh>(chunk));
            this->set(&chunk, Renderer::generate_mesh(chunk));
        });
    };
    VoxelStructure* getStructure() { return _structure; }

    void draw(unsigned int primitive, glm::mat4 modelmatrix, Shader* shader) {
        this->forEachMesh([&](VoxelChunk& chunk, VoxelMesh* mesh) {
            // mesh->drawInstances(primitive, modelmatrix, shader);
            mesh->draw(primitive, modelmatrix, shader);
        });
    }

    template<typename Func>
    void forEachMesh(Func func) {
        for (auto& [chunk, mesh] : _map) {
            func(*chunk, mesh.get());
        }
    }

    void set(VoxelChunk* chunk, std::unique_ptr<VoxelMesh> mesh) {
        _map[chunk] = std::move(mesh);
    }

    VoxelMesh* get(VoxelChunk* chunk) const {
        auto it = _map.find(chunk);
        return (it != _map.end()) ? it->second.get() : nullptr;
    }

    void remove(VoxelChunk* chunk) {
        auto it = _map.find(chunk);
        if (it != _map.end()) {
            _map.erase(it);
        }
    }

private:
    std::unordered_map<VoxelChunk*, std::unique_ptr<VoxelMesh>> _map;
    VoxelStructure* _structure;
};