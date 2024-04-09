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
    Mesh* _mesh;
    VoxelModel* _voxelModel;
    glm::ivec3 _sizes;

    bool _visible;
public:
    ModelObject(VoxelModel* model);
    ~ModelObject();

    virtual void draw(glm::mat4 matrix, Shader* shader);

    void setMesh(Mesh* mesh);

    /**
     * @brief Получить указатель на объект Mesh
     * 
     * @return Mesh* Указатель на объект Mesh или nullptr
     */
    Mesh* getMesh();

    VoxelModel *getVoxelModel();

    void setVisible(bool visible);
    bool isVisible();
};
