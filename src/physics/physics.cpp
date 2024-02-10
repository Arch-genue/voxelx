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

std::vector<line> PhysicsObject::getVertices() {
    BoxCollider *collider = getCollider();
    BoundingBox *boundingbox = getCollider()->getBoundingbox();

    if (collider == nullptr) {
        std::cerr << "ERROR: PhysicsObject::getVertices() called with no collider!\n";
    }
    if (boundingbox == nullptr) {
        std::cerr << "ERROR: PhysicsObject::getVertices() called with no boundingbox!\n";
    }

    boundingbox->min.x = -0.5f;
    boundingbox->max.x = collider->getSize().x - 0.5f;

    boundingbox->min.y = -0.5f;
    boundingbox->max.y = collider->getSize().y - 0.5f;

    // boundingbox->zMin = getPosition().z - 0.5f;
    boundingbox->max.z = collider->getSize().z - 0.5f;

    std::vector<line> vertices;

    line vert0 = {
        glm::vec3(boundingbox->min.x, boundingbox->min.y, -0.5f),
        glm::vec3(boundingbox->min.x, boundingbox->max.y, -0.5f)
    };

    line vert1 = {
        glm::vec3(boundingbox->min.x, boundingbox->min.y, -0.5f),
        glm::vec3(boundingbox->max.x, boundingbox->min.y, -0.5f)
    };

    line vert2 = {
        glm::vec3(boundingbox->max.x, boundingbox->min.y, -0.5f),
        glm::vec3(boundingbox->max.x, boundingbox->min.y, boundingbox->max.z)
    };

    line vert3 = {
        glm::vec3(boundingbox->min.x, boundingbox->min.y, -0.5f),
        glm::vec3(boundingbox->min.x, boundingbox->min.y, boundingbox->max.z)
    };

    line vert4 = {
        glm::vec3(boundingbox->min.x, boundingbox->min.y, boundingbox->max.z),
        glm::vec3(boundingbox->max.x, boundingbox->min.y, boundingbox->max.z)
    };

    line vert5 = {
        glm::vec3(boundingbox->min.x, boundingbox->max.y, -0.5f),
        glm::vec3(boundingbox->max.x, boundingbox->max.y, -0.5f)
    };

    line vert6 = {
        glm::vec3(boundingbox->max.x, boundingbox->max.y, -0.5f),
        glm::vec3(boundingbox->max.x, boundingbox->max.y, boundingbox->max.z)
    };

    line vert7 = {
        glm::vec3(boundingbox->min.x, boundingbox->max.y, -0.5f),
        glm::vec3(boundingbox->min.x, boundingbox->max.y, boundingbox->max.z)
    };

    line vert8 = {
        glm::vec3(boundingbox->min.x, boundingbox->max.y, boundingbox->max.z),
        glm::vec3(boundingbox->max.x, boundingbox->max.y, boundingbox->max.z)
    };

    line vert9 = {
        glm::vec3(boundingbox->max.x, boundingbox->max.y, -0.5f),
        glm::vec3(boundingbox->max.x, boundingbox->min.y, -0.5f)
    };

    line vert10 = {
        glm::vec3(boundingbox->min.x, boundingbox->min.y, boundingbox->max.z),
        glm::vec3(boundingbox->min.x, boundingbox->max.y, boundingbox->max.z)
    };

    line vert11 = {
        glm::vec3(boundingbox->max.x, boundingbox->max.y, boundingbox->max.z),
        glm::vec3(boundingbox->max.x, boundingbox->min.y, boundingbox->max.z)
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

    return vertices;
}