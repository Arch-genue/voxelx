/**
 * @file boxcollider.h
 * @author Vlad Kartsaev
 * @brief Collider implementation 
 * @version 0.1
 * @date 2024-02-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <glm/glm.hpp>

struct BoundingBox {
    float xMin, xMax, yMin, yMax, zMin, zMax;
};

class BoxCollider{
private:
    BoundingBox* _boundingbox;
    glm::vec3 _size;
public:
    BoxCollider(BoundingBox& _boundingbox);
    ~BoxCollider();

    BoundingBox* getBoundingbox() const;
    
    void setSize(glm::vec3 size);
    glm::vec3 getSize() const;
};