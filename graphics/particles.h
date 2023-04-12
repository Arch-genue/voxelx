#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <vector>
#include <algorithm>
#include "vox_renderer.h"
#include "mesh.h"
#include "shader.h"
#include "../voxels/voxel.h"

using namespace glm;

class VoxelRenderer;
class Mesh;
class Shader;
class voxel_m;

struct particle_m {
    float vx, vy, vz;
    float lifetime;
    vec3 velocity;
    vec3 position;
};

class VoxelParticles {
    Shader* sh;
    VoxelRenderer* renderer;
    vec3 m_gravity;
public:
    VoxelParticles(int bufferSize, VoxelRenderer* render, Shader* shader);
    ~VoxelParticles();

    void Update(float dt);
    void addParticle(const particle_m& particle);
    void draw() const;

private:
    std::vector<particle_m> m_particles;
};