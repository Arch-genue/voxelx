#pragma once

#include <glm/glm.hpp>
#include "../graphics/renderer.h"

class Mesh;
class Shader;
class Renderer;

// Создаем структуру Ray, чтобы представить луч
struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;
};

class GameObject {
    //?OBJECTS
    Renderer* render;
    Mesh* mesh;
    Shader* shader;

    //?POSITIONS
    glm::mat4 modelmatrix;
    glm::vec3 position;
    glm::vec3 lastposition;
    glm::vec3 rotAxis;
    float rotAngle;
    glm::vec3 scaling;

    //! VoxPhysics v0.0.1
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 impulse;
    float impulseTime;

    //?PROPERTIES
    float mass;
    bool collider;
public:
    GameObject(Renderer* rndr, _voxels voxels, Shader *sh);
    ~GameObject();

    void translate(float val, glm::vec3 vec);
    void rotate(float val, glm::vec3 rot);
    void scale(float val, glm::vec3 scalevec);

    //Pos
    void setPosition(glm::vec3 pos);
    glm::vec3 getPosition();
    void setRotation(float angle, glm::vec3 rot);
    void setScale(glm::vec3 scalevec);
    glm::mat4 getMatrix();

    void draw();
    void updatePhysics(float deltaTime);

    void setImpulse(glm::vec3 force);
    void applyForce(glm::vec3 force);

    //Voxel operations
    void setVoxelState(uint8_t i, bool state);
    bool getVoxel(glm::vec3 pos);

    bool testraycast(glm::vec3 rayOrigin, glm::vec3 rayDirection, float maxDistance);
    bool rayCast(glm::vec3 a, glm::vec3 dir, float maxDist, glm::vec3& end, glm::vec3& norm, glm::vec3& iend);
};