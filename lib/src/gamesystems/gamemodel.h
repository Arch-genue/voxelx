/**
 * @file gamemodel.h
 * @author Vlad Kartsaev
 * @brief GameModel класс, который связывает и обьединяет классы:
 * VoxelStructure, MeshModel, Shader, Texture в один объект
 * Предоставляет единую систему контроля за моделью
 * @version 0.1
 * @date 2025-09-05
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include "voxels/voxelstructure.h"
#include "graphics/meshmodel.h"
#include "graphics/shader.h"
#include "reactphysics3d/reactphysics3d.h"

#include <memory>
#include <string>

struct PhysicsInfo {
    bool enabled = false;

    float mass = 0.4;
    float bouncing = 0;
    float friction = 0;
    
    std::unique_ptr<rp3d::CollisionShape> collision_type = nullptr;
};


struct GameModelInfo {
    std::string name;
    std::string path;
    
    std::string model_file;
    std::string model_type;

    std::string shader_fragment;
    std::string shader_vertex;

    PhysicsInfo physics;

    GameModelInfo() = default;
    GameModelInfo(const GameModelInfo& other): 
        name(other.name), path(other.path), model_file(other.model_file), model_type(other.model_type),
        shader_fragment(other.shader_fragment), shader_vertex(other.shader_vertex)
    {
        // копируем PhysicsInfo вручную
        physics.enabled = other.physics.enabled;
        physics.mass = other.physics.mass;
        physics.bouncing = other.physics.bouncing;
        physics.friction = other.physics.friction;

        // collision_type нужно копировать через clone() или создать новый объект
        // if (other.physics.collision_type) {
        //     // rp3d::CollisionShape — абстрактный класс, поэтому нужен clone()
        //     physics.collision_type = std::unique_ptr<rp3d::CollisionShape>(
        //         other.physics.collision_type->clone() // предполагаем, что clone() возвращает CollisionShape*
        //     );
        // } else {
        physics.collision_type = nullptr;
        // }
    }
};

class GameModel {
public:
    GameModel(const std::filesystem::path& path);
    GameModel(const GameModel& other);

    VoxelStructure* getStructure() const { return _structure; }
    MeshModel* getModel() const { return _model; }
    void getTexture() const;

    void setEditable(bool editable) { _editable = editable; }
    bool getEditable() const { return _editable; }
private:
    std::string _path;
    bool _editable;

    // Основные данные модели
    GameModelInfo _gamemodelinfo;

    // Связанные объекты
    VoxelStructure* _structure;
    MeshModel* _model;

    uint8_t load_gamemodel(const std::string& name);
};