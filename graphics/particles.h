#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <vector>
#include <algorithm>
#include "vox_renderer.h"
#include "mesh.h"
#include "shader.h"
#include "../voxels/voxel.h"

class VoxelRenderer;
class Mesh;
class Shader;
class voxel_m;

//! MEMORY LEAKS
class VoxelParticles {
    VoxelRenderer* renderer;
    Mesh* mesh;
    Shader* sh;
    _voxels* voxels;
    
    glm::vec3 m_gravity;
public:
    VoxelParticles(int bufferSize, VoxelRenderer* render, Shader* shader);
    ~VoxelParticles();

    void update(float dt);
    void addParticle(const voxel_m& particle);
    void draw();    
};