#include "voxelparticles.h"
#include "../loaders/resourcemanager.h"
#include "renderer.h"
#include "../utilities/logger.h"

#include <fstream>

std::mt19937 rng(std::random_device{}());

uint8_t one_iter = 0;
VoxelParticles::VoxelParticles(std::string name, uint16_t particlecount, glm::vec3 position): _name(name), _particlecount(particlecount), _position(position) {
    _particles = ResourceManager::getParticles(name);
    if (_particles == nullptr) {
        return;
    }
    
    _gravity = glm::vec3(0, -9.8, 0);
    _particlesarray = new ParticlesModel(glm::vec3(0));

    glm::mat3x3 matrix = _particles->color;
    __red = glm::vec3(matrix[0][0], matrix[0][1], matrix[0][2]);
    __green = glm::vec3(matrix[1][0], matrix[1][1], matrix[1][2]);
    __blue = glm::vec3(matrix[2][0], matrix[2][1], matrix[2][2]);


    // std::cout << "Vec1: (" << red.x << ", " << red.y << ", " << red.z << ")" << std::endl;
    // std::cout << "Vec2: (" << green.x << ", " << green.y << ", " << green.z << ")" << std::endl;
    // std::cout << "Vec3: (" << blue.x << ", " << blue.y << ", " << blue.z << ")" << std::endl;

    pos_generator = std::uniform_real_distribution<float>(-1000.0f, 1000.0f);
    vel_generator = std::uniform_real_distribution<float>(_particles->force.x, _particles->force.z);
    redclr_generator = std::uniform_real_distribution<float>(__red.x, __red.z);
    greenclr_generator = std::uniform_real_distribution<float>(__green.x, __green.z);
    blueclr_generator = std::uniform_real_distribution<float>(__blue.x, __blue.z);
    lifetime_generator = std::uniform_real_distribution<float>(3.0f, 6.0f);

    for (int i = 0; i < particlecount; ++i) {
        Voxel* particle = new Voxel;
        // particle.position = glm::vec3(0, 0, 0);
        particle->setLifeTime(lifetime_generator(rng));
        // particle.visible = true;
        particle->setPosition(position);
        _particlesarray->addVoxel(particle); 
        calculateAnimation(_particlesarray->getVoxel(i));
    }

}

VoxelParticles::~VoxelParticles() {
    delete _particlesarray;
}
void VoxelParticles::setReady(bool ready) {
    _voxelsReady = ready;
}

void VoxelParticles::setPosition(glm::vec3 pos) {
    _position = pos;
}
glm::vec3 VoxelParticles::getPosition() {
    return _position;
}
void VoxelParticles::setSize(float size) {
    m_size = size;
}

void VoxelParticles::update(float deltaTime) {
    for (uint16_t i = 0; i < _particlecount; i++) {
        Voxel* voxel = _particlesarray->getVoxel(i);
        glm::vec3 position = voxel->getPosition();
        // std::cout << position.y << " " << (position + _voxelsarray->getVoxel(i)->getVelocity() * deltaTime).y << " " << deltaTime << std::endl;
        if (voxel->getLifeTime() >= 0.0f) {
            voxel->setLifeTime(voxel->getLifeTime() - deltaTime);
            if (position.y > 2) {
                // voxel->setPosition(position + _particlesarray->getVoxel(i)->getVelocity() * deltaTime);
                // _voxelsarray->setVoxelPosition(i, glm::vec3(10));
                // _voxelsarray->setVoxelPosition(i, position + _voxelsarray->getVoxel(i)->getVelocity() * deltaTime);
            } else {
                // position.y = 1;
            }
            // _voxelsarray->setVoxelPosition(i, position);
            // std::cout << "I: " << i << " " << _voxelsarray->voxels[i].position.y << std::endl;
        
            
        } else {
            _particlesarray->getVoxel(i)->setLifeTime(_particlesarray->getVoxel(i)->getLifeTime() - deltaTime);
            calculateAnimation(_particlesarray->getVoxel(i));    
        }
    }

    _mesh = Renderer::render(_particlesarray);
    _mesh->draw(GL_TRIANGLES);
    delete _mesh;
}

void VoxelParticles::calculateAnimation(Voxel* voxel) {
    glm::vec3 position = voxel->getPosition();
    voxel->setPosition(_position + glm::vec3(pos_generator(rng), 0, pos_generator(rng)));
    // voxel->setVelocity(glm::vec3(0, vel_generator(rng) + _gravity.y, 0));

    float _tmpclrred = (__red.x == -1) ? __red.y : redclr_generator(rng);
    float _tmpclrgreen = (__red.x == -1) ? __red.y : redclr_generator(rng);
    float _tmpclrblue = (__red.x == -1) ? __red.y : redclr_generator(rng);
    // std::cout << "RED: " << __red.x << " " << __red.y << " " << __red.z << "\n";
    // std::cout << "GREEN: " << __green.x << " " << __green.y << " " << __green.z << "\n";
    // std::cout << "BLUE: " << __blue.x << " " << __blue.y << " " << __blue.z << "\n";
    // std::cout << "TMP: " << _tmpclrred << " " << _tmpclrgreen << " " << _tmpclrblue << "\n";

    voxel->setColor(glm::vec4(_tmpclrred, _tmpclrgreen, _tmpclrblue, 1.0f));
    voxel->setLifeTime(lifetime_generator(rng));
    voxel->setVisible(true);
    // vox->visible = true;
}

// void VoxelParticles::draw(float deltaTime) {
//     if (_recursive && !_voxelsReady) {
//         for (size_t i = 0; i < voxels->voxels.size(); i++) {
//             calculateAnimation(type, &voxels->voxels[i]);
//         }
//     }

//     for (uint16_t i = 0; i < voxels->voxels.size(); i++) {
//         if (voxels->voxels[i].lifetime >= 0.0f) {
//             voxels->voxels[i].position += voxels->voxels[i].velocity * deltaTime;
        
//             voxels->voxels[i].lifetime -= deltaTime;
//             _val += 0.01f;

//             if (type == EFFECT_CURSED_FLAME) {
//                 voxels->voxels[i].clr = glm::vec4(voxels->voxels[i].clr.x, voxels->voxels[i].clr.y - 0.01f, voxels->voxels[i].clr.z + 0.005f, 1.0f);
//             }
//         } else {
//             _val = 0.0f;
//             if (!_recursive){
//                 calculateAnimation(type, &voxels->voxels[i]);
//             } else {

//             }
//         }
//     }

//     if ( !_voxelsReady ) return;

//     //* DRAW
//     mesh = Renderer::render(voxels);
//     mesh->draw(GL_TRIANGLES);
//     delete mesh;
// }

// //? Cursed Black-Violet Flame
// void VoxelParticles::effect_cursed_flame(voxel_m* vox) {
//     vox->position = m_position + glm::vec3(pos_generator(rng), 0, pos_generator(rng));
//     vox->velocity = glm::vec3(0, vel_generator(rng), 0);
//     float clr = clr_generator(rng);
//     vox->clr = glm::vec4(clr, clr, clr, 1.0f);
//     vox->lifetime = lifetime_generator(rng);
// }

// //? Dead Flame
// void VoxelParticles::effect_dead_flame(voxel_m* vox) {
//     vox->position = m_position + glm::vec3(pos_generator(rng), 0, pos_generator(rng));
//     vox->velocity = glm::vec3(0, vel_generator(rng), 0);
//     float clr = clr_generator(rng);
//     vox->clr = glm::vec4(clr, clr, clr, 1.0f);
//     vox->lifetime = lifetime_generator(rng);
// }

// //? Red Flame
// void VoxelParticles::effect_flame(voxel_m* vox) {
//     vox->position = m_position + glm::vec3(pos_generator(rng), 0, pos_generator(rng));
//     vox->velocity = glm::vec3(0, vel_generator(rng), 0);
//     vox->clr = glm::vec4(1.0f, clr_generator(rng), 0.0f, 1.0f);
//     vox->lifetime = lifetime_generator(rng);
// }

// //? Explosion
// void VoxelParticles::effect_explosion(voxel_m* vox) {
//     // test += 1.0f;
//     int clr = clr_generator(rng);
//     // vox->position = m_position; // + glm::vec3(pos_generator(rng), 0, pos_generator(rng));
//     vox->velocity = glm::vec3(vel_generator(rng), vel_generator(rng), vel_generator(rng));
//     vox->clr = glm::vec4(clr, clr, clr, 1.0f);
//     vox->lifetime = lifetime_generator(rng);
// }

Particles* VoxelParticles::load_voxel_particles(std::string filename) {
    std::ifstream in(filename);
    Particles* _particles = new Particles;
    if (in.is_open()) {
        std::string line;
        uint16_t vi = 0;

        std::string str[2];

        glm::vec3 red;
        glm::vec3 green;
        glm::vec3 blue;

        while (getline(in, line)) {
            vi++;
            if (vi - 1 == 0) continue;

            splitvalue(str, line); // x y z clr

            // if (str[0] == "") continue;
            char *position;
            float* varvalue = new float[2];

            if (str[0] == "colorred" || str[0] == "colorgreen" || str[0] == "colorblue" || str[0] == "force") {
                float* varvalue = random_keyword_parse(str[1]);
                if (varvalue == nullptr) continue;
                glm::vec3 val;
                if (varvalue[1] == -1500000) {
                    val = glm::vec3(-1, varvalue[0], -1);
                } else {
                    val = glm::vec3(varvalue[0], 0, varvalue[1]);
                }
                if (str[0] == "colorred") red = val;
                else if (str[0] == "colorgreen") green = val;
                else if (str[0] == "colorblue") blue = val;
                else if (str[0] == "force") _particles->force = val;
            }
            else if (str[0] == "position") {
                _particles->position = str[1];
            } else if (str[0] == "gravity") {
                _particles->gravity = (str[1] == "true");
            } else if (str[0] == "recursive") {
                _particles->recursive = (str[1] == "true");
            } else if (str[0] == "start_force") {
                _particles->start_force = (str[1] == "true");
            }
            delete[] varvalue;
        }
        _particles->color = glm::mat3x3(red, green, blue);
        in.close();
    } else return nullptr;
    return _particles;
}