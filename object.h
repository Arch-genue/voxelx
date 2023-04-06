#pragma once

#include <glm/glm.hpp>

class Mesh;
class Shader;

//using namespace glm;

class Object {
    glm::vec3 position;
    glm::vec3 chgpos;
    glm::mat4 matrix;
    float rotAngle;
    glm::vec3 rotAxis;
public:
    Object(glm::mat4 *mat);
    ~Object();

    void translate(glm::vec3 vec);
    void rotate(float angle, glm::vec3 rot);
    void scale();

    //Pos
    void setPosition(glm::vec3 pos);
    glm::vec3 getPosition();
    glm::mat4 getMatrix();

    void draw(Mesh *m, Shader *sh);

    //Voxel operations
    void setVoxelState(uint8_t i, bool state);
    bool getVoxelState(uint8_t i);
};
//position = glm::vec3(0, 0, 0);