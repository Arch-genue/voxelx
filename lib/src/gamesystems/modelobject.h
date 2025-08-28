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

#include "../graphics/meshmodel.h"

class ModelObject {
private:
    MeshModel* _meshmodel;

    bool _visible;
public:
    ModelObject(MeshModel* original_model);
    ~ModelObject() {
        delete _meshmodel;
    }

    virtual void draw(glm::mat4 matrix, Shader* shader);

    MeshModel* getMeshModel() const { return _meshmodel; }

    void setVisible(bool visible) { _visible = visible; }
    bool isVisible() const { return _visible; }
};