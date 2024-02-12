#include "boxcollider.h"

BoxCollider::BoxCollider(GameObject* gameobject, glm::vec3 min, glm::vec3 max) : _gameobject(gameobject), _min(min), _max(max) {}
BoxCollider::~BoxCollider() {}

void BoxCollider::setMin(glm::vec3 position) { 
    _min = position;
}

void BoxCollider::setMax(glm::vec3 size) {
    _max = size;
}

glm::vec3 BoxCollider::getMin() { 
    return  _min;
}

glm::vec3 BoxCollider::getMax() {
    return _max;
}