/**
 * @file octotree.h
 * @author Vlad Kartsaev
 * @brief Implementation of Octotree
 * @version 0.1
 * @date 2024-02-29
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../mesh.h"
#include "../../utils.h"

#include "../../gamesystems/gameobject.h"

class OctoNode {
private:
    float* _vertices;

    Mesh* _mesh;

    uint16_t _depth;
public:
    OctoNode* _parent;
    OctoNode* _children[8];

    GameObject* _gameobject;

    glm::ivec3 _min;
    glm::ivec3 _max;

    OctoNode(OctoNode* parent, glm::vec3 mina, glm::vec3 maxa);
    ~OctoNode();

    GameObject* find();

    size_t getNodeCount();

    void insert(uint16_t maxdepth);
    
    void remove(GameObject *gameobject);

    void setDepth(uint16_t depth);
    uint16_t getDepth();

    void draw();
};

class OctoTree {
private:
    OctoNode* _root;

    uint16_t _maxdepth;
public:
    OctoTree(glm::vec3 min, glm::vec3 max);
    ~OctoTree();

    OctoNode* getRoot() const { return _root; };

    void setMaxDepth(uint16_t depth);

    void generate();

    void draw();
};
