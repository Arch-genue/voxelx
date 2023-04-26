#include "physics.h"

PhysicsObject::PhysicsObject(glm::vec3 position, _physicsmodel type, float mass)
 : position(position), type(type), mass(mass) {}

PhysicsObject::~PhysicsObject() {}

void PhysicsObject::setPosition(glm::vec3 pos) {
    position = pos;
}
void PhysicsObject::setType(_physicsmodel ptype) {
    type = ptype;
}
void PhysicsObject::setMass(float pmass) {
    mass = pmass;
}

void PhysicsObject::applyForce(glm::vec3 force) {
    acceleration += (force / mass) * 100.0f;
}

void PhysicsObject::update(float deltaTime) {
    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;
    acceleration = glm::vec3(0);
}