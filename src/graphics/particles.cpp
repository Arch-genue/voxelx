#include "particles.h"
#include "../loaders/resourcemanager.h"
#include "renderer.h"

#include <iostream>

std::mt19937 rng(std::random_device{}());

VoxelParticles::VoxelParticles(_effects ptype, int bufferSize) {
    m_gravity = glm::vec3(0, -9.8, 0);
    voxels = new _voxels;
    
    setType(ptype);
    setPosition(glm::vec3(0));
    setSize(0.05f);
    _val = 0.0f;

    for (int i = 0; i < bufferSize; ++i) {
        voxel_m particle;
        particle.lifetime = 0.0f;
        particle.visible = true;
        //addParticle(particle);
        voxels->voxels.push_back(particle); 
    }

    draw(0);
}

VoxelParticles::~VoxelParticles() {
    delete voxels;
    //delete mesh;
}

void VoxelParticles::setType(_effects ptype) {
    type = ptype;
    glm::vec2 posvals(0.0f);
    glm::vec2 velvals(0.0f);
    glm::vec2 clrvals(0.0f);
    glm::vec2 lifetimevals(0.0f);

    switch(ptype) {
        case EFFECT_IMPORT:
            //flame(it);
        break;
        case EFFECT_FLAME:
            posvals = glm::vec2(-1.0f, 1.0f);
            velvals = glm::vec2(7.5f, 30.0f);
            clrvals = glm::vec2(0.05f, 0.3f);
            lifetimevals = glm::vec2(0.05f, 0.3f);
        break;
        case EFFECT_CURSED_FLAME:
            posvals = glm::vec2(-1.0f, 1.0f);
            velvals = glm::vec2(7.5f, 30.0f);
            clrvals = glm::vec2(0.05f, 0.3f);
            lifetimevals = glm::vec2(0.2f, 0.4f);
        break;
        case EFFECT_VOMIT:
            posvals = glm::vec2(0.0f, 1.0f);
            velvals = glm::vec2(10.0f, 20.0f);
            clrvals = glm::vec2(0.05f, 0.3f);
            lifetimevals = glm::vec2(0.1f, 1.0f); 
        break;
        case EFFECT_WATER:
            posvals = glm::vec2(-1.0f, 1.0f);
            velvals = glm::vec2(0.1f, 2.0f);
            clrvals = glm::vec2(0.05f, 0.3f);
            lifetimevals = glm::vec2(0.5f, 2.0f); 
        break;
        default:
        break;
    }

    pos_generator = std::uniform_real_distribution<float>(posvals.x, posvals.y);
    vel_generator = std::uniform_real_distribution<float>(velvals.x, velvals.y);
    clr_generator = std::uniform_real_distribution<float>(clrvals.x, clrvals.y);
    lifetime_generator = std::uniform_real_distribution<float>(lifetimevals.x, lifetimevals.y);
}

void VoxelParticles::setPosition(glm::vec3 pos) {
    m_position = pos;
}
void VoxelParticles::setSize(float size) {
    m_size = size;
}

void VoxelParticles::draw(float deltaTime) {
    for (size_t i = 0; i < voxels->voxels.size(); i++) {
        if (voxels->voxels[i].lifetime >= 0.0f) {
            voxels->voxels[i].position += voxels->voxels[i].velocity * deltaTime;
        
            voxels->voxels[i].lifetime -= deltaTime;
            _val += 0.01f;
            if (type == EFFECT_CURSED_FLAME) {
                voxels->voxels[i].clr = glm::vec4(voxels->voxels[i].clr.x, voxels->voxels[i].clr.y-0.01f, voxels->voxels[i].clr.z+0.005f, 1.0f);
            }
        } else {
            _val = 0.0f;
            switch(type) {
                case EFFECT_IMPORT:

                break;
                case EFFECT_FLAME:
                    effect_flame(&voxels->voxels[i]);
                break;
                case EFFECT_CURSED_FLAME:
                    effect_cursed_flame(&voxels->voxels[i]); //? Innovation
                break;
                case EFFECT_VOMIT:
                    effect_vomit(&voxels->voxels[i]);
                break;
                case EFFECT_WATER:
                    effect_water(&voxels->voxels[i]);
                break;
                default:
                    continue;
                break;
            }
            
        }
    }
    //* DRAW
    ResourceManager::getShader("voxel")->uniformMatrix("model", glm::scale(glm::mat4(1.0f), glm::vec3(0.05f)));
    mesh = Renderer::render(voxels);
    mesh->draw(GL_TRIANGLES);
    delete mesh;
}

//? Cursed Black-Violet Flame
void VoxelParticles::effect_cursed_flame(voxel_m* vox) {
    vox->position = m_position + glm::vec3(pos_generator(rng), 0, pos_generator(rng));
    vox->velocity = glm::vec3(0, vel_generator(rng), 0);
    float clr = clr_generator(rng);
    vox->clr = glm::vec4(clr, clr, clr, 1.0f);
    vox->lifetime = lifetime_generator(rng);
}

//? Dead Flame
// void VoxelParticles::effect_cursed_flame(voxel_m* vox) {
//     vox->position = m_position + glm::vec3(pos_generator(rng), 0, pos_generator(rng));
//     vox->velocity = glm::vec3(0, vel_generator(rng), 0);
//     float clr = clr_generator(rng);
//     vox->clr = glm::vec4(clr, clr, clr, 1.0f);
//     vox->lifetime = lifetime_generator(rng);
// }

//? Red Flame
void VoxelParticles::effect_flame(voxel_m* vox) {
    vox->position = m_position + glm::vec3(pos_generator(rng), 0, pos_generator(rng));
    vox->velocity = glm::vec3(0, vel_generator(rng), 0);
    vox->clr = glm::vec4(1.0f, clr_generator(rng), 0.0f, 1.0f);
    vox->lifetime = lifetime_generator(rng);
}

//? Vomit
void VoxelParticles::effect_vomit(voxel_m* vox) {
    vox->position = m_position + glm::vec3(pos_generator(rng), pos_generator(rng), pos_generator(rng));
    vox->velocity = m_gravity / 2.0f + glm::vec3(0, 0, vel_generator(rng));
    vox->clr = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
    vox->lifetime = lifetime_generator(rng);
}

//? Water
void VoxelParticles::effect_water(voxel_m* vox) {
    vox->position = m_position + glm::vec3(pos_generator(rng), 0, pos_generator(rng));
    vox->velocity = glm::vec3(0, vel_generator(rng), 0);
    vox->clr = glm::vec4(0.0f, 0.0, 0.0f, 1.0f);
    vox->lifetime = lifetime_generator(rng);
}