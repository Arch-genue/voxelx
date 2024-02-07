#pragma once

#include "../utils.h"
#include <vector>

/**
 * @brief Класс описывающий воксель и все его параметры
 * 
 */
class Voxel {
    public:
        Voxel();
        Voxel(glm::vec3 position);
        Voxel(glm::vec3 position, glm::vec4 color);
        Voxel(glm::vec3 position, glm::vec3 velocity, glm::vec4 color);
        
        ~Voxel();

        void setPosition(glm::vec3 position);
        glm::vec3 getPosition();

        void setVelocity(glm::vec3 velocity);
        glm::vec3 getVelocity();

        void setColor(glm::vec4 color);
        glm::vec4 getColor();

        void setVisible(bool visible);
        bool isVisible();

        void setLifeTime(float lifetime);
        float getLifeTime();

    private:
        glm::vec3 _position;
        glm::vec3 _velocity;
        glm::vec4 _color;

        float _lifetime;

        bool _visible;
};

/**
 * @brief Класс описывающий модель состоящую из вокселей как контейнер
 * 
 */
class VoxelModel {
    public:
        VoxelModel();
        ~VoxelModel();

        void setLight(uint8_t side, glm::vec3 light);
        glm::vec3 getLight(uint8_t side);
        glm::vec3* getLightArray();

        void addVoxel(Voxel* voxel);

        int getVoxelsCount();

        void setVoxel(int num, Voxel* voxel);
        Voxel* getVoxel(int num);

        void setSize(glm::vec3 size);
        glm::vec3 getSize();

        void setRenderSide(std::string renderside);
        std::string getRenderSide();

        //? Voxel Functions
        void setVoxelPosition(int num, glm::vec3 position);
        glm::vec3 getVoxelPosition(int num);
    private:
        std::vector<Voxel*> _voxels;
        glm::vec3 _size;
        std::string _renderside;

        glm::vec3 _light[6];
};

// struct voxel_m {
//     glm::vec3 position;
//     glm::vec3 velocity;
//     glm::vec4 clr;
//     float size;

//     float lifetime;
//     bool visible;
// };

// struct _voxels {
//     std::vector<voxel_m> voxels;
//     glm::vec3 m_size;
//     std::string renderSide;

//     glm::vec3 light[6];
// };

extern VoxelModel* load_model(std::string filename, const char* type);
extern VoxelModel* genVoxel();