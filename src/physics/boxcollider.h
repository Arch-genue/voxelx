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
// #include <vector>

class GameObject;

class BoxCollider {
private:
    GameObject* _gameobject;

    glm::vec3 _size;

    glm::vec3 _min; // Минимальные координаты BoundingBox
    glm::vec3 _max;
public:
    BoxCollider(GameObject* gameobject, glm::vec3 min, glm::vec3 max);
    ~BoxCollider();

    GameObject* getGameObject() { return _gameobject; }
    
    void setMin(glm::vec3 position);
    glm::vec3 getMin();

    void setMax(glm::vec3 size);
    glm::vec3 getMax();
};

// Узел красно-черного дерева BoundingBox
class Node {
private:
    BoxCollider* _box;
    bool _isRed; // Цвет узла (true - красный, false - черный)
    Node* _left;
    Node* _right;

public:
    Node(BoxCollider* box) : _box(box), _isRed(true), _left(nullptr), _right(nullptr) {}
    
    void setIsRed(bool isRed) { _isRed = isRed; };
    bool isRed() const { return _isRed; };

    BoxCollider* getBoxCollider() const { return _box; }

    void setLeft(Node* left) {  _left = left; };
    void setRight(Node* right) {  _right = right; };

    Node* getLeft() const { return _left; };
    Node* getRight() const { return _right; };
};