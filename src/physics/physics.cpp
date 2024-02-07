#include "physics.h"

#include "../graphics/mesh.h"
#include "../voxels/voxel.h"
#include "../gamesystems/gamemanager.h"
class Voxel;
class Mesh;
class GameManager;

PhysicsObject::PhysicsObject(GameObject* gmobj)
 : pGameObject(gmobj), position(glm::vec3(0)), type(_physicsmodel::NO_PHYSICS), mass(0) {}
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

void PhysicsObject::explode(uint16_t explodeForce) {
    GameManager* gm = pGameObject->getGameManager();
    if (gm != nullptr) {
        // VoxelParticles* explosionEffect = new VoxelParticles(EFFECT_EXPLOSION, 70, false);
        // std::cout << "obj.pos " << pGameObject->getPosition().x << " " << pGameObject->getPosition().y << " " << pGameObject->getPosition().z << "\n";
        // std::cout << "effect.pos " << explosionEffect->getPosition().x << " " << explosionEffect->getPosition().y << " " << explosionEffect->getPosition().z << "\n";
        // explosionEffect->setPosition(glm::vec3(15.0f, 30.0f, 1.5f));
        // gm->addVoxelParticles(explosionEffect);
        // explosionEffect->setReady(true);
    }

    // Mesh* mesh = pGameObject->getMesh();
    // size_t len = mesh->getVoxels()->voxels.size();
    
    // for (int i = 0; i < len; i++) {
    //     voxel_m vox = mesh->getVoxel(i);
    //     glm::vec3 oldpos = vox.position;
    //     vox.velocity = glm::vec3(0, 200.0f, 0);
    //     // vox.clr = glm::vec4(1, 1, 1, 1);
    //     mesh->setVoxel(i, vox);
        
    //     // mesh->update();
        
    // }
    
    // Mesh* newmesh = Renderer::render(mesh->getVoxels());
    // pGameObject->setMesh(newmesh);
}

void PhysicsObject::update(float deltaTime) {
    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;
    acceleration = glm::vec3(0);
}