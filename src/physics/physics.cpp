#include "physics.h"

#include "../graphics/mesh.h"
#include "../voxels/voxel.h"
#include "../gamesystems/gamemanager.h"

class Voxel;
class Mesh;
class GameManager;

PhysicsObject::PhysicsObject(GameObject *gmobj, BoxCollider *collider, float mass)
    : _gameobject(gmobj), _collider(collider), _position(glm::vec3(0)), _physics(PHYSICS::NO_PHYSICS) {
        setVelocity(glm::vec3(0));
        setAcceleration(glm::vec3(0));
        setMass(mass);
}
PhysicsObject::PhysicsObject(glm::vec3 position, PHYSICS physics, float mass) : _position(position),  _physics(physics), _mass(mass) {
    _velocity = glm::vec3(0);
    _acceleration = glm::vec3(0);
}
PhysicsObject::~PhysicsObject() {}

void PhysicsObject::setCollider(BoxCollider *collider) {
    _collider = collider;
}
BoxCollider *PhysicsObject::getCollider() {
    return _collider;
}

GameObject *PhysicsObject::getGameObject() {
    return _gameobject;
}

void PhysicsObject::setPosition(glm::vec3 position) {
    _position = position;
}
glm::vec3 PhysicsObject::getPosition() {
    return _position;
}

void PhysicsObject::setVelocity(glm::vec3 velocity) {
    _velocity = velocity;
}
glm::vec3 PhysicsObject::getVelocity() {
    return _velocity;
}

void PhysicsObject::setAcceleration(glm::vec3 acceleration) {
    _acceleration = acceleration;
}
glm::vec3 PhysicsObject::getAcceleration() {
    return _acceleration;
}

void PhysicsObject::setPhysics(PHYSICS physics) {
    _physics = physics;
}
PHYSICS PhysicsObject::getPhysics() {
    return _physics;
}

void PhysicsObject::setMass(float mass) {
    _mass = mass;
}
float PhysicsObject::getMass()
{
    return _mass;
}

void PhysicsObject::applyForce(glm::vec3 force){
    _acceleration += (force / _mass) * 100.0f;
}
void PhysicsObject::stopForce()
{
    _acceleration = glm::vec3(0);
}

void PhysicsObject::explode(uint16_t explodeForce)
{
    GameManager *gm = _gameobject->getGameManager();
    if (gm != nullptr)
    {
        // VoxelParticles* explosionEffect = new VoxelParticles(EFFECT_EXPLOSION, 70, false);
        // std::cout << "obj.pos " << pGameObject->getPosition().x << " " << pGameObject->getPosition().y << " " << pGameObject->getPosition().z << "\n";
        // std::cout << "effect.pos " << explosionEffect->getPosition().x << " " << explosionEffect->getPosition().y << " " << explosionEffect->getPosition().z << "\n";
        // explosionEffect->setPosition(glm::vec3(15.0f, 30.0f, 1.5f));
        // gm->addVoxelParticles(explosionEffect);
        // explosionEffect->setReady(true);
    }
}

bool PhysicsObject::isGrounded()
{
    return _ground;
}

bool PhysicsObject::checkGround(glm::vec3 &position, glm::vec3 &normal) {
    if (_position.y <= 1) {
        position = glm::vec3(_position.x, 1, _position.z);
        normal = glm::vec3(0, 1, 0);
        _ground = true;
        return true;
    }
    _ground = false;
    return false;
}

void PhysicsObject::update(float deltaTime) {
    _velocity += _acceleration * deltaTime;
    _position += _velocity * deltaTime;
    _acceleration = glm::vec3(0);
}

float *PhysicsObject::getVertices()
{
    BoxCollider *collider = getCollider();
    BoundingBox *boundingbox = getCollider()->getBoundingbox();
    boundingbox->xMin = getPosition().x - 0.5f;
    boundingbox->xMax = collider->getSize().x - 0.5f;

    boundingbox->yMax = collider->getSize().y - 0.5f;

    // boundingbox->zMin = getPosition().z - 0.5f;
    boundingbox->zMax = collider->getSize().z - 0.5f;
    float *vertices = new float[72] {
        // x      y       z
        boundingbox->xMin,
        -0.5f,
        -0.5f,
        boundingbox->xMin,
        boundingbox->yMax,
        -0.5f,

        //* Нижняя полоса 1
        boundingbox->xMin,
        -0.5f,
        -0.5f,
        boundingbox->xMax,
        -0.5f,
        -0.5f,

        //* Нижняя полоса 2
        boundingbox->xMax,
        -0.5f,
        -0.5f,
        boundingbox->xMax,
        -0.5f,
        boundingbox->zMax,

        //* Нижняя полоса 3
        boundingbox->xMin,
        -0.5f,
        -0.5f,
        boundingbox->xMin,
        -0.5f,
        boundingbox->zMax,

        //* Нижняя полоса 4
        boundingbox->xMin,
        -0.5f,
        boundingbox->zMax,
        boundingbox->xMax,
        -0.5f,
        boundingbox->zMax,

        //* Верхняя полоса 1
        boundingbox->xMin,
        boundingbox->yMax,
        -0.5f,
        boundingbox->xMax,
        boundingbox->yMax,
        -0.5f,

        //* Верхняя полоса 2
        boundingbox->xMax,
        boundingbox->yMax,
        -0.5f,
        boundingbox->xMax,
        boundingbox->yMax,
        boundingbox->zMax,

        //* Верхняя полоса 3
        boundingbox->xMin,
        boundingbox->yMax,
        -0.5f,
        boundingbox->xMin,
        boundingbox->yMax,
        boundingbox->zMax,

        //* Верхняя полоса 4
        boundingbox->xMin,
        boundingbox->yMax,
        boundingbox->zMax,
        boundingbox->xMax,
        boundingbox->yMax,
        boundingbox->zMax,

        boundingbox->xMax,
        boundingbox->yMax,
        -0.5f,
        boundingbox->xMax,
        -0.5f,
        -0.5f,

        boundingbox->xMin,
        -0.5f,
        boundingbox->zMax,
        boundingbox->xMin,
        boundingbox->yMax,
        boundingbox->zMax,

        boundingbox->xMax,
        boundingbox->yMax,
        boundingbox->zMax,
        boundingbox->xMax,
        -0.5f,
        boundingbox->zMax,
    };
    return vertices;
}