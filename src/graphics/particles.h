#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <vector>
#include <random>
#include <algorithm>

#include "renderer.h"
#include "mesh.h"
#include "shader.h"
#include "../voxels/voxel.h"

class Renderer;
class Mesh;
class Shader;
class voxel_m;

enum _effects {
    EFFECT_IMPORT,

    EFFECT_FLAME,
    EFFECT_VOMIT,
    EFFECT_WATER,
};

struct _particles {

};

class VoxelParticles {
    //? Objects
    Renderer* renderer;
    Mesh* mesh;
    _voxels* voxels;
    
    //? 
    glm::vec3 m_gravity;
    glm::vec3 m_position;
    float m_size;
    _effects type;

    //? Random Generators
    std::uniform_real_distribution<float> pos_generator;
    std::uniform_real_distribution<float> vel_generator;
    std::uniform_real_distribution<float> clr_generator;
    std::uniform_real_distribution<float> lifetime_generator;
public:
    VoxelParticles(Renderer* render, _effects ptype, int bufferSize);
    ~VoxelParticles();

    void addParticle(voxel_m& particle);

    void setType(_effects ptype);
    void setPosition(glm::vec3 pos);
    void setSize(float size);

    void draw(float deltaTime);

    //Default Particles
    void effect_flame(voxel_m* vox);
    void effect_vomit(voxel_m* vox);
    void effect_water(voxel_m* vox);
};