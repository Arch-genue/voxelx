#include "boxcollider.h"

BoxCollider::BoxCollider(BoundingBox& boundingBox) : _boundingbox(&boundingBox) {}
BoxCollider::~BoxCollider() {}

BoundingBox *BoxCollider::getBoundingbox() const { return _boundingbox; }

void BoxCollider::setSize(glm::vec3 size) {
    _size = size;
}
glm::vec3 BoxCollider::getSize() const { return _size; }
