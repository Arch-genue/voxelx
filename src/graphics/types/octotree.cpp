#include "octotree.h"

#include <GL/glew.h>

OctoNode::OctoNode(OctoNode* parent, glm::vec3 mina, glm::vec3 maxa) {
    _parent = parent;
    _depth = 1;

    for (uint8_t i = 0; i < 8; i++) {
        _children[i] = nullptr;
    }
    glm::vec3 min;
    glm::vec3 max;

    _min = mina;
    _max = maxa;

    min.x = mina.x - 0.5f;
    max.x = mina.x + maxa.x - 0.5f;

    min.y = mina.y - 0.5f;
    max.y = mina.y + maxa.y - 0.5f;

    min.z = mina.z - 0.5f;
    max.z = mina.z + maxa.z - 0.5f;

    std::vector<line> vertices;

    line vert0 = {
        glm::vec3(min.x, min.y, min.z),
        glm::vec3(min.x, max.y, min.z)
    };

    line vert1 = {
        glm::vec3(min.x, min.y, min.z),
        glm::vec3(max.x, min.y, min.z)
    };

    line vert2 = {
        glm::vec3(max.x, min.y, min.z),
        glm::vec3(max.x, min.y, max.z)
    };

    line vert3 = {
        glm::vec3(min.x, min.y, min.z),
        glm::vec3(min.x, min.y, max.z)
    };

    line vert4 = {
        glm::vec3(min.x, min.y, max.z),
        glm::vec3(max.x, min.y, max.z)
    };

    line vert5 = {
        glm::vec3(min.x, max.y, min.z),
        glm::vec3(max.x, max.y, min.z)
    };

    line vert6 = {
        glm::vec3(max.x, max.y, min.z),
        glm::vec3(max.x, max.y, max.z)
    };

    line vert7 = {
        glm::vec3(min.x, max.y, min.z),
        glm::vec3(min.x, max.y, max.z)
    };

    line vert8 = {
        glm::vec3(min.x, max.y, max.z),
        glm::vec3(max.x, max.y, max.z)
    };

    line vert9 = {
        glm::vec3(max.x, max.y, min.z),
        glm::vec3(max.x, min.y, min.z)
    };

    line vert10 = {
        glm::vec3(min.x, min.y, max.z),
        glm::vec3(min.x, max.y, max.z)
    };

    line vert11 = {
        glm::vec3(max.x, max.y, max.z),
        glm::vec3(max.x, min.y, max.z)
    };
    
    vertices.push_back(vert0);

    vertices.push_back(vert1);
    vertices.push_back(vert2);

    vertices.push_back(vert3);
    vertices.push_back(vert4);

    vertices.push_back(vert5);
    vertices.push_back(vert6);

    vertices.push_back(vert7);
    vertices.push_back(vert8);

    vertices.push_back(vert9);
    vertices.push_back(vert10);

    vertices.push_back(vert11);

    float vertices1[72];

    uint d = 0;
    for (size_t i = 0; i < vertices.size(); i++) {
        vertices1[d] 	 = vertices[i].vertex1.x;
        vertices1[d + 1] = vertices[i].vertex1.y;
        vertices1[d + 2] = vertices[i].vertex1.z;

        vertices1[d + 3] = vertices[i].vertex2.x;
        vertices1[d + 4] = vertices[i].vertex2.y;
        vertices1[d + 5] = vertices[i].vertex2.z;
        
        d += 6;
    }
    
    int attrs[2] = { 3,  0 };
    _mesh = new Mesh(vertices1, 24, attrs);
}

OctoNode::~OctoNode() {
    delete _mesh;
}

void OctoNode::insert(uint16_t maxdepth) {
    if (_depth > maxdepth) {
        return;
    }

    if (_children[0] == nullptr) {
        glm::ivec3 size = _max / 2;

        uint16_t i = 0;
        for (uint16_t z = 0; z < 2; z++) {
            for (uint16_t x = 0; x < 2; x++) {
                for (uint16_t y = 0; y < 2; y++) {
                    glm::ivec3 pos(size.x * x, size.y * y, size.z * z);

                    _children[i] = new OctoNode(_parent, _min + pos, size);
                    _children[i]->_parent = this;
                    _children[i]->setDepth(_depth + 1);
                    _children[i]->insert(maxdepth);

                    // errorprint("OctoNode", std::to_string(iterator) + " " + std::to_string(xn) + " " + std::to_string(yn) + " " + std::to_string(zn), MSGINFO);
                    i++;
                }
            }
        }
    }
}

size_t OctoNode::getNodeCount() {
    size_t count = 0;
    for (auto& child : _children) {
        if (child != nullptr) {
            count++;
            count += child->getNodeCount();
        }
    }
    return count;
}

void OctoNode::draw() {
    if (_children[0] == nullptr) {
        _mesh->draw(GL_LINES);
        return;
    }
    for (uint8_t i = 0; i < 8; i++) {
        if (_children[i] != nullptr) {
            _children[i]->draw();
        }
    }
}

void OctoNode::setDepth(uint16_t depth) {
    _depth = depth;
}
uint16_t OctoNode::getDepth() {
    return _depth;
}

OctoTree::OctoTree(glm::vec3 min, glm::vec3 max) {
    _root = new OctoNode(_root, min, max);
    // _root->min = min;
    // _root->max = max;
}

OctoTree::~OctoTree() {
    delete _root;
}

void OctoTree::setMaxDepth(uint16_t depth) {
    _maxdepth = depth;
}

void OctoTree::generate() {
    _root->setDepth(1);
    _root->insert(_maxdepth);
}

void OctoTree::draw() {
    _root->draw();
}


