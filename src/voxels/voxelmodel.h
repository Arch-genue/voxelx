/**
 * @file voxelmodel.h
 * @author Vlad Kartsaev
 * @brief Implementation of the VoxelModel and Octree class.
 * @version 0.1
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "voxel.h"
#include "../utilities/vtype.h"
#include "../utilities/logger.h"

class Mesh;

/**
 * @brief класс для хранения вокселей с доступом по координатам.
 * 
 */
class VoxelModel {
private:
    std::string _name;
    vtype::array3<int32_t, Voxel*> *_voxels;

    std::string _renderside;
    glm::vec3 _size;

    //* Предзагруженный стандартный меш
    Mesh* _pmesh;
    
public:
    VoxelModel(glm::i16vec3 size, int depth = 5);
    ~VoxelModel();

    void setName(std::string name);
    std::string getName();

    virtual void addVoxel(Voxel* voxel);
    virtual void removeVoxel(glm::ivec3 position);
    virtual void setVoxelVisible(glm::ivec3 position, bool visible);

    Voxel* getVoxel(glm::ivec3 position);

    void fillColor(glm::vec4 color);

    void setMesh(Mesh* mesh);
    Mesh* getMesh();

    void setRenderSide(std::string& renderside) const { renderside = _renderside; }
    std::string getRenderSide() const { return _renderside; }

    void setSize(glm::ivec3 size) { _size = size; }
    glm::ivec3 getSize() const { return _size; }
};

extern VoxelModel *load_model(std::string filename, const char *type);
extern VoxelModel *genVoxel();