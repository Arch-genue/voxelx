/**
 * @file modelobject.h
 * @author Vlad Kartsaev
 * @brief Model object with draw
 * @version 0.1
 * @date 2024-03-28
 * 
 * @copyright Copyright (c) 2024 Forever.
 * 
 */
#pragma once

#include "GL/glew.h"
#include "../graphics/mesh.h"
#include "../voxels/voxelmodel.h"

class ModelObject {
private:
    // std::unique_ptr<VoxelModel> _voxelModel;
    VoxelModel* _voxelModel;
    glm::ivec3 _sizes;

    bool _visible;
public:
    ModelObject(VoxelModel* original_model);
    ~ModelObject();

    VoxelModel* getVoxelModel() const;

    virtual void draw(glm::mat4 matrix, Shader* shader);

    void setVisible(bool visible);
    bool isVisible();
};
