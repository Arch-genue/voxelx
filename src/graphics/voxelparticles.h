#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <random>
#include <algorithm>

#include "mesh.h"
#include "../voxels/particlesmodel.h"

class Mesh;
class Shader;
class Voxel;

// class VoxelParticle()
struct Particles {
    glm::mat3x3 color;

    std::string position;

    bool recursive;
    bool gravity;

    bool start_force;
    char* force_direction;
    glm::vec3 force;

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
    Mesh* _mesh;
    ParticlesModel* _particlesarray;
    Particles* _particles;
    
    //? Properties
    glm::vec3 _gravity;
    glm::vec3 _position;
    float m_size;
    _effects type;

    //? Random Generators
    std::uniform_real_distribution<float> pos_generator;
    std::uniform_real_distribution<float> vel_generator;
    std::uniform_real_distribution<float> redclr_generator;
    std::uniform_real_distribution<float> greenclr_generator;
    std::uniform_real_distribution<float> blueclr_generator;
    std::uniform_real_distribution<float> lifetime_generator;

    //--sys
    float _val;
    bool _voxelsReady;
    std::string _name;
    uint16_t _particlecount;

    //--Type
    bool _recursive;

    //! Temporary effect vars
    glm::vec3 __red;
    glm::vec3 __green;
    glm::vec3 __blue;
    
public:
    VoxelParticles(std::string name, uint16_t particlecount, glm::vec3 position);
    // VoxelParticles(_effects ptype, int bufferSize, bool recursive = false);
    ~VoxelParticles();
    static Particles* load_voxel_particles(std::string filename);

    void setReady(bool ready);

    void addParticle(Voxel& particle);

    void setType(_effects ptype);
    void setPosition(glm::vec3 pos);
    glm::vec3 getPosition();
    void setSize(float size);

    void update(float deltaTime);

    void calculateAnimation(Voxel* vox);

    //? Default Particles
    // void effect_flame(voxel_m* vox);
    // void effect_cursed_flame(voxel_m* vox);
    // void effect_dead_flame(voxel_m* vox);
    
    // void effect_vomit(voxel_m* vox);
    // void effect_water(voxel_m* vox);

    // void effect_explosion(voxel_m* vox);
};