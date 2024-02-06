#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <random>
#include <algorithm>

#include "mesh.h"
#include "../voxels/voxel.h"

class Mesh;
class Shader;
class voxel_m;

// class VoxelParticle()
struct Particles {
    glm::vec3 color;
    char* position;

    bool recursive;
    bool gravity;

    bool start_force;
    char* force_direction;
    float force;

    char* lifetime;
};

enum _effects {
    EFFECT_IMPORT,

    EFFECT_FLAME,
    EFFECT_CURSED_FLAME,
    EFFECT_DEAD_FLAME,
    EFFECT_VOMIT,
    EFFECT_WATER,
    EFFECT_EXPLOSION,
};

class VoxelParticles {
    //? Objects
    Mesh* mesh;
    _voxels* voxels;
    
    //? Properties
    glm::vec3 m_gravity;
    glm::vec3 m_position;
    float m_size;
    _effects type;

    //? Random Generators
    std::uniform_real_distribution<float> pos_generator;
    std::uniform_real_distribution<float> vel_generator;
    std::uniform_real_distribution<float> clr_generator;
    std::uniform_real_distribution<float> lifetime_generator;

    //--sys
    float _val;
    bool _voxelsReady; 

    //--Type
    bool _recursive;
    
public:
    VoxelParticles(Particles* particles);
    VoxelParticles(_effects ptype, int bufferSize, bool recursive = false);
    ~VoxelParticles();
    static Particles* load_voxel_particles(std::string filename);

    void setReady(bool ready);

    void addParticle(voxel_m& particle);

    void setType(_effects ptype);
    void setPosition(glm::vec3 pos);
    glm::vec3 getPosition();
    void setSize(float size);

    void draw(float deltaTime);

    void calculateAnimation(_effects type, voxel_m* vox);

    //? Default Particles
    void effect_flame(voxel_m* vox);
    void effect_cursed_flame(voxel_m* vox);
    void effect_dead_flame(voxel_m* vox);
    
    void effect_vomit(voxel_m* vox);
    void effect_water(voxel_m* vox);

    void effect_explosion(voxel_m* vox);
};