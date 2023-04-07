#include "gameobject.h"

#include <GL/glew.h>
#include <glm/ext.hpp>
#include "graphics/mesh.h"
#include "graphics/shader.h"

//#include <iostream>

GameObject::GameObject(glm::mat4 *mat) {
    modelmatrix = (*mat);
    position = glm::vec3(0);
    rotAngle = 0.0f;
    rotAxis = glm::vec3(1);
    scaling = glm::vec3(0.1f);
}
GameObject::~GameObject() {}

void GameObject::draw(Mesh *m, Shader *sh) {
    modelmatrix = glm::scale(modelmatrix, scaling);
    modelmatrix = glm::translate(modelmatrix, position);
    modelmatrix = glm::rotate(modelmatrix, rotAngle, rotAxis);

    sh->uniformMatrix("model", modelmatrix);
    m->draw(GL_TRIANGLES);

    modelmatrix = glm::mat4(1.0f);
}

void GameObject::translate(float val, glm::vec3 vec) {
    position += vec * val;
}

void GameObject::rotate(float angle, glm::vec3 rot) {
    rotAngle += angle;
    rotAxis = rot; 
}

void GameObject::scale(float scale, glm::vec3 scalevec) {
    scaling += scalevec*scale;
    std::cout << (scaling.x) << std::endl;
}

void GameObject::setPosition(glm::vec3 pos) {
    position = pos;
}
glm::vec3 GameObject::getPosition() {
    return position;
}
void GameObject::setRotation(float angle, glm::vec3 rot) {
    rotAngle = angle;
    rotAxis = rot; 
}
void GameObject::setScale(glm::vec3 scalevec) {
    scaling = scalevec;
}

glm::mat4 GameObject::getMatrix() {
    return modelmatrix;
}