#include "object.h"

#include <GL/glew.h>
#include <glm/ext.hpp>
#include "graphics/mesh.h"
#include "graphics/shader.h"

//#include <iostream>

Object::Object(glm::mat4 *mat) {
    matrix = (*mat);
    position = glm::vec3(0);
    chgpos = glm::vec3(0);
    rotAngle = 0.0f;
    rotAxis = glm::vec3(1);
}
Object::~Object() {}

void Object::draw(Mesh *m, Shader *sh) {
    matrix = glm::scale(matrix, glm::vec3(0.7f, 0.7f, 0.7f));
    matrix = glm::translate(matrix, position);
    matrix = glm::rotate(matrix, rotAngle, rotAxis);
    sh->uniformMatrix("model", matrix);
    m->draw(GL_TRIANGLES);

    matrix = glm::mat4(1.0f);
}

void Object::translate(glm::vec3 vec) {
    position = vec;
}

void Object::rotate(float angle, glm::vec3 rot) {
    rotAngle = angle;
    rotAxis = rot; 
}
void Object::scale() {}

void Object::setPosition(glm::vec3 pos) {
    position = pos;
}
glm::vec3 Object::getPosition() {
    return position;
}
glm::mat4 Object::getMatrix() {
    return matrix;
}