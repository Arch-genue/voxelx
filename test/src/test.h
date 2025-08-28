#pragma once

#include <algorithm> // для std::max
#include <cmath>     // для std::abs
#include <set>

// #include "delaunator.hpp"
#include "FastNoiseLite.h"

// std::mt19937 rndg(static_cast<unsigned int>(std::time(nullptr)));
// std::uniform_real_distribution<float> dist(-1.0f, 1.0f);


class MapGenerator {
public:
    MapGenerator(glm::ivec3 size) {
        _size = size;
    }

    VoxelModel* generateMap() {
        VoxelModel* voxmodel = new VoxelModel();
        voxmodel->setMinSize(glm::ivec3(-_size.x / 2, -_size.y / 2, -_size.z / 2));
        voxmodel->setSize(glm::ivec3(_size.x / 2, _size.y / 2, _size.z / 2));

        FastNoiseLite noise;
        noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
        noise.SetFrequency(0.01f);
        noise.SetFractalOctaves(4);
        noise.SetFractalLacunarity(2.0f);
        noise.SetFractalGain(0.5f);

        const int maxHeight = _size.y / 2 - 5; // максимальная высота карты
        const int waterLevel = maxHeight / 3; // уровень воды
        const int hillLevel = 2 * maxHeight / 3; // уровень холмов

        for (int x = -_size.x / 2; x < _size.x / 2; ++x) {
            for (int z = -_size.z / 2; z < _size.z / 2; ++z) {
                float n = noise.GetNoise(float(x), float(z)); // [-1,1]
                n = (n + 1.0f) / 2.0f; // [0,1]

                int height = int(n * maxHeight);
                height = std::clamp(height, 0, maxHeight - 1);

                Voxel voxel;
                voxel.position = glm::ivec3(x, height, z);

                // Цвет в зависимости от уровня
                if (height < waterLevel) {
                    // вода
                    voxel.color = glm::vec4(0.0f, 0.3f, 0.8f, 1.0f);
                } else if (height < hillLevel) {
                    // равнина
                    float t = float(height - waterLevel) / float(hillLevel - waterLevel);
                    glm::vec3 baseColor = glm::vec3(0.2f, 0.8f, 0.2f); // зелёная равнина
                    glm::vec3 finalColor = baseColor * (0.5f + 0.5f * t);
                    voxel.color = glm::vec4(finalColor, 1.0f);
                } else {
                    // холмы / горы
                    float t = float(height - hillLevel) / float(maxHeight - hillLevel);
                    glm::vec3 baseColor = glm::vec3(0.5f, 0.5f, 0.5f); // серые горы
                    glm::vec3 finalColor = glm::mix(baseColor, glm::vec3(1.0f,1.0f,1.0f), t); // вершины белеют
                    voxel.color = glm::vec4(finalColor, 1.0f);
                }

                voxmodel->setVoxel(x, height, z, voxel);
            }
        }

        return voxmodel;
    }

    glm::vec3 hsl2rgb(float h, float s, float l) {
        float c = (1.0f - std::fabs(2.0f*l - 1.0f)) * s;
        float hp = h * 6.0f; // h в [0..1], умножаем на 6 для сегмента
        float x = c * (1.0f - std::fabs(fmod(hp, 2.0f) - 1.0f));

        float r=0, g=0, b=0;
        if      (0 <= hp && hp < 1) { r=c; g=x; b=0; }
        else if (1 <= hp && hp < 2) { r=x; g=c; b=0; }
        else if (2 <= hp && hp < 3) { r=0; g=c; b=x; }
        else if (3 <= hp && hp < 4) { r=0; g=x; b=c; }
        else if (4 <= hp && hp < 5) { r=x; g=0; b=c; }
        else if (5 <= hp && hp < 6) { r=c; g=0; b=x; }

        float m = l - 0.5f * c;
        return glm::vec3(r+m, g+m, b+m);
    }

private:
    glm::ivec3 _size;
};