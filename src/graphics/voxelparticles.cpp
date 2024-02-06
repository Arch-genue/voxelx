#include "voxelparticles.h"
#include "../loaders/resourcemanager.h"
#include "renderer.h"
#include "../utils.cpp"

#include <iostream>
#include <fstream>

std::mt19937 rng(std::random_device{}());

VoxelParticles::VoxelParticles(Particles* particles) {
    setType(EFFECT_IMPORT);
}

VoxelParticles::VoxelParticles(_effects ptype, int bufferSize, bool recursive) : _recursive(recursive) {
    m_gravity = glm::vec3(0, -9.8, 0);
    voxels = new _voxels;
    for(uint16_t i = 0; i < 6; i++) voxels->light[i] = glm::vec3(1);
    
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

    _voxelsReady = recursive;
}

VoxelParticles::~VoxelParticles() {
    delete voxels;
    //delete mesh;
}
void VoxelParticles::setReady(bool ready) {
    _voxelsReady = ready;
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
        case EFFECT_DEAD_FLAME: case EFFECT_FLAME:
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
        case EFFECT_EXPLOSION:
            posvals = glm::vec2(-1.0f, 1.0f);
            velvals = glm::vec2(-30.0f, 30.0f);
            clrvals = glm::vec2(0.1f, 0.3f);
            lifetimevals = glm::vec2(0.5f, 0.75f);  
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
glm::vec3 VoxelParticles::getPosition() {
    return m_position;
}
void VoxelParticles::setSize(float size) {
    m_size = size;
}

void VoxelParticles::draw(float deltaTime) {
    if (_recursive && !_voxelsReady) {
        for (size_t i = 0; i < voxels->voxels.size(); i++) {
            calculateAnimation(type, &voxels->voxels[i]);
        }
    }

    for (uint16_t i = 0; i < voxels->voxels.size(); i++) {
        if (voxels->voxels[i].lifetime >= 0.0f) {
            voxels->voxels[i].position += voxels->voxels[i].velocity * deltaTime;
        
            voxels->voxels[i].lifetime -= deltaTime;
            _val += 0.01f;

            if (type == EFFECT_CURSED_FLAME) {
                voxels->voxels[i].clr = glm::vec4(voxels->voxels[i].clr.x, voxels->voxels[i].clr.y - 0.01f, voxels->voxels[i].clr.z + 0.005f, 1.0f);
            }
        } else {
            _val = 0.0f;
            if (!_recursive){
                calculateAnimation(type, &voxels->voxels[i]);
            } else {

            }
        }
    }

    if ( !_voxelsReady ) return;

    //* DRAW
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
void VoxelParticles::effect_dead_flame(voxel_m* vox) {
    vox->position = m_position + glm::vec3(pos_generator(rng), 0, pos_generator(rng));
    vox->velocity = glm::vec3(0, vel_generator(rng), 0);
    float clr = clr_generator(rng);
    vox->clr = glm::vec4(clr, clr, clr, 1.0f);
    vox->lifetime = lifetime_generator(rng);
}

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

//? Explosion
void VoxelParticles::effect_explosion(voxel_m* vox) {
    // test += 1.0f;
    int clr = clr_generator(rng);
    // vox->position = m_position; // + glm::vec3(pos_generator(rng), 0, pos_generator(rng));
    vox->velocity = glm::vec3(vel_generator(rng), vel_generator(rng), vel_generator(rng));
    vox->clr = glm::vec4(clr, clr, clr, 1.0f);
    vox->lifetime = lifetime_generator(rng);
}

void VoxelParticles::calculateAnimation(_effects type, voxel_m* vox) {
    switch(type) {
        case EFFECT_IMPORT:

        break;
        case EFFECT_FLAME:
            effect_flame(vox);
        break;
        case EFFECT_CURSED_FLAME:
            effect_cursed_flame(vox); //? Innovation
        break;
        case EFFECT_DEAD_FLAME:
            effect_dead_flame(vox);
        break;
        case EFFECT_VOMIT:
            effect_vomit(vox);
        break;
        case EFFECT_WATER:
            effect_water(vox);
        break;
        case EFFECT_EXPLOSION:
            effect_explosion(vox);
        break;
    }
}

Particles* VoxelParticles::load_voxel_particles(std::string filename) {
    std::ifstream in(filename);
    Particles* _particles;
    if (in.is_open()) {
        std::string line;
        uint16_t vi = 0;

        std::string str[2];

        while (getline(in, line)) {
            vi++;
            if (vi - 1 == 0) {
                std::cout << "VOXPART Header read" << " \n";    
                continue;
            }
            // if (str[0] == "") continue;
            char *position;

            splitvalue(str, line); // x y z clr
            if (str[0] == "colorred") {
                std::cout << str[1] << " \n";
            } else if (str[0] == "colorgreen") {
                
            } else if (str[0] == "colorblue") {

            } else if (str[0] == "position") {
                
            } else if (str[0] == "gravity") {
                _particles->gravity = (str[1] == "true");
            } else if (str[0] == "recursive") {
                _particles->recursive = (str[1] == "true");
            } else if (str[0] == "start_force") {
                _particles->start_force = (str[1] == "true");
            }
            // std::cout << str[0] << " \n";
        }
        in.close();
    } else return nullptr;
    return _particles;
}