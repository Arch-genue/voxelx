/**
 * @file particlesmodel.h
 * @author Vlad Kartsaev
 * @brief Implementation of particles model
 * @version 0.1
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

#include "voxelmodel.h"

class ParticlesModel : public VoxelModel {
public:
    ParticlesModel(glm::vec3 size);
    ~ParticlesModel();

    void addVoxel(Voxel *voxel) override;

    size_t getVoxelsCount();

    void setVoxel(int num, Voxel *voxel);
    Voxel* getVoxel(int num);
    Voxel* getVoxel(glm::vec3 position);

    void setSize(glm::vec3 size);
    glm::vec3 getSize();

    //? Voxel Functions
    void setVoxelPosition(int num, glm::vec3 position);
    glm::vec3 getVoxelPosition(int num);

private:
    std::vector<Voxel*> _voxels;
    glm::vec3 _size;
};