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

enum Collision {NO_COLLISION, SIMPLE_COLLISION};

class GameObject {
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

        void attachCamera(Camera* cam, glm::vec3 stdpos);

        void setCollision(Collision coll);
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
    private:
        //? OBJECTS
        Mesh* _mesh;
        Mesh* _boundingbox;
        Camera* _camera;
        VoxelModel* _voxelmodel;
        
        //? GameManager global object
        GameManager* _gm;
        
        //? POSITIONS
        glm::mat4 _modelmatrix;
        glm::vec3 _position;
        glm::vec3 _lastposition;
        glm::vec3 _rotAxis;
        float _rotAngle;
        glm::vec3 _scaling;
        glm::vec3 _campos;

        //! Voxel Physics
        PhysicsObject* _physobject;
        bool _rigidbody;
        bool _onGround;

        //? PROPERTIES
        bool _visible;
        bool _hidden;
        float _mass;

        //? Collision
        Collision _collision;
        glm::vec3 _boundbox_size;
        BOUNDINGBOX _bbox;
};