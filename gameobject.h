#pragma once

#include <glm/glm.hpp>

class Mesh;
class Shader;

class GameObject {
    glm::mat4 modelmatrix;

    glm::vec3 position;
    glm::vec3 rotAxis;
    float rotAngle;
    glm::vec3 scaling;
public:
    GameObject(glm::mat4 *mat);
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

    void draw(Mesh *m, Shader *sh);

    //Voxel operations
    void setVoxelState(uint8_t i, bool state);
    bool getVoxelState(uint8_t i);
};