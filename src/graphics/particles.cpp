#include "particles.h"

#include <iostream>

#include <random>

// Создание генератора случайных чисел
std::mt19937 rng(std::random_device{}());
// Определение распределения для координат
std::uniform_real_distribution<float> pos_dist(-1.0f, 1.0f);
// Определение распределения для скоростей
std::normal_distribution<float> vel_dist(0.0f, 3.0f);

std::normal_distribution<float> clr_dist(0.0f, 1.0f);
std::normal_distribution<float> life_dist(0.5f, 1.5f);

VoxelParticles::VoxelParticles(int bufferSize, VoxelRenderer* render, Shader* shader) {
    renderer = render;
    sh = shader;
    m_gravity = glm::vec3(0, -9.8, 0);

    voxels = new _voxels;
    voxels->voxels.reserve(bufferSize);
    mesh = renderer->render(voxels);
}
VoxelParticles::~VoxelParticles() {
    //delete mesh;
}

void VoxelParticles::update(float dt) {
    for (auto it = voxels->voxels.begin(); it != voxels->voxels.end(); it++) {
        if (it->lifetime >= 0.0f) {
            it->position += 15.0f * it->velocity * dt;
            it->lifetime -= dt;
        } else {
            it->position = glm::vec3(pos_dist(rng), pos_dist(rng), pos_dist(rng));
            it->velocity = glm::vec3(vel_dist(rng), vel_dist(rng), vel_dist(rng));
            it->clr = glm::vec4(clr_dist(rng), clr_dist(rng), clr_dist(rng), 1.0f);
            it->lifetime = life_dist(rng);
            it--;
        }
    }
}

void VoxelParticles::addParticle(const voxel_m& particle) { 
    voxels->voxels.push_back(particle); 
}

void VoxelParticles::draw() {
    //mesh->clear();
    delete mesh;
    mesh = renderer->render(voxels);
    sh->uniformMatrix("model", glm::scale(glm::mat4(1.0f), glm::vec3(0.1f)));
    mesh->draw(GL_TRIANGLES);
}