#include "particles.h"

#include <iostream>

VoxelParticles::VoxelParticles(int bufferSize, VoxelRenderer* render, Shader* shader) {
    renderer = render;
    sh = shader;
    m_gravity = vec3(0, -9.8, 0);
    m_particles.reserve(bufferSize);
}
VoxelParticles::~VoxelParticles() {}

void VoxelParticles::Update(float dt) {
    for (auto it = m_particles.begin(); it != m_particles.end(); it++) {
        particle_m& p = *it;
        if (p.lifetime >= 0.0f) {
            p.position.x += 15.0f * p.velocity.x * dt;
            p.position.y += 15.0f * p.velocity.y * dt;
            p.position.z += 15.0f * p.velocity.z * dt;

            // Обновление позиции
            p.position += p.velocity * dt;
            p.lifetime -= dt;
        } else {
            m_particles.erase(it);
            particle_m newParticle;
            newParticle.position.x = 0.0f;
            newParticle.position.y = 0.0f;
            newParticle.position.z = 0.0f;
            newParticle.velocity.x = rand() / static_cast<float>(RAND_MAX) - 0.5f;
            newParticle.velocity.y = rand() / static_cast<float>(RAND_MAX) - 0.5f;
            newParticle.velocity.z = rand() / static_cast<float>(RAND_MAX) - 0.5f;
            newParticle.lifetime = 5.0f;
            m_particles.push_back(newParticle);
            it--;
        }
    }

    // for (auto& particle : m_particles) {
    //     particle.position.x += 15.0f * particle.velocity.x * dt;
    //     particle.position.y += 15.0f * particle.velocity.y * dt;
    //     particle.position.z += 15.0f * particle.velocity.z * dt;

    //     // Обновление позиции
    //     particle.position += particle.velocity * dt;
    //     particle.lifetime -= dt;
        
    //     if (particle.lifetime <= 0.0f ) {

    //     }
    // }

    // m_particles.erase(
    //     std::remove_if(m_particles.begin(), 
    //         m_particles.end(),
    //         [](const particle_m& p) { 
    //             return p.lifetime <= 0.0f; 
    //             }
    //     ),
    //     m_particles.end()
    // );
}

void VoxelParticles::addParticle(const particle_m& particle) { 
    m_particles.push_back(particle); 
}

void VoxelParticles::draw() const {
    // отрисовка всех частиц с помощью вашего воксельного движка
    for (auto& particle : m_particles) {
        voxel_m* voxel = new voxel_m;
        voxel->position = particle.position;
        voxel->clr = vec4(1.0f, 0.5f, 0.0f, 1.0f);
        Mesh *m = renderer->voxelRender(voxel);

        mat4 model(1.0f);
        model = scale(model, vec3(0.05f));

        sh->uniformMatrix("model", model);
        m->draw(GL_TRIANGLES);
        delete m;
    }
}