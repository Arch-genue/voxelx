#pragma once

#include <glm/glm.hpp>
#include "../graphics/renderer.h"
#include "../physics/physics.h"

class Mesh;
class Shader;
class Renderer;
class PhysicsObject;
class GameManager;

struct BOUNDINGBOX {
    glm::vec3 min;
    glm::vec3 max;
};

enum _collision {NO_COLLISION, SIMPLE_COLLISION};

class GameObject {
    //? OBJECTS
    Mesh* mesh;
    Camera* camera;
    _voxels* voxels;

    GameManager* gm;
    
    //? POSITIONS
    glm::mat4 modelmatrix;
    glm::vec3 position;
    glm::vec3 lastposition;
    glm::vec3 rotAxis;
    float rotAngle;
    glm::vec3 scaling;
    vec3 campos;

    //! Voxel Physics
    PhysicsObject* _physobject;
    bool rigidbody;
    bool onGround;

    //? PROPERTIES
    bool visible;
    bool hidden;
    float mass;

    //? Collision
    _collision collision;
    glm::vec3 _boundbox_size;
    BOUNDINGBOX bbox;
public:
    GameObject(const char* model);
    ~GameObject();
    void setGameManager(GameManager* gamemanager);

    void setLight(glm::vec3 *light);
    glm::vec3* getLight();

    PhysicsObject* getPhysics();

    void translate(float val, glm::vec3 vec);
    void rotate(float val, glm::vec3 rot);
    void scale(float val, glm::vec3 scalevec);

    //* Pos
    void setPosition(glm::vec3 pos);
    glm::vec3 getPosition();
    void setRotation(float angle, glm::vec3 rot);
    void setScale(glm::vec3 scalevec);
    glm::mat4 getMatrix();

    void setVisible(bool vis);
    void setHidden(bool hid);

    void draw();
    void updatePhysics(float deltaTime);

    void attachCamera(Camera* cam, vec3 stdpos);

    void setCollision(_collision coll);
    bool getCollision();

    void setRigidBody(bool rigid);
    bool getRigidBody();

    //* Voxel operations
    void setVoxelState(uint8_t i, bool state);
    bool getVoxel(glm::vec3 pos);

    bool raycast(glm::vec3 pos, glm::vec3 dir, float maxDist, glm::vec3& end, glm::vec3& norm, glm::vec3& iend);

    void setMesh(Mesh* newmesh);
    Mesh* getMesh();

    GameManager* getGameManager();

    glm::ivec3 checkCollision(BOUNDINGBOX b);
    bool checkGround();
    BOUNDINGBOX getBBOX();
};