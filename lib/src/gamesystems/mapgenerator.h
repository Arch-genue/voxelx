#pragma once

#include <algorithm> // для std::max
#include <cmath>     // для std::abs
#include <set>
#include <random>

// #include "delaunator.hpp"
#include "vendor/FastNoiseLite.h"

std::random_device rd;            // для получения случайного числа от системы
std::mt19937 gen(rd());           // генератор
std::uniform_int_distribution<> dist(0, 100000);  // диапазон seed

class MapGenerator {
public:
    std::vector<float> _heightarray;

    MapGenerator(glm::ivec3 size) {
        _size = size;
        _heightarray.reserve(_size.x * _size.z);
    }

    void createTree(VoxelStructure* voxmodel, int x, int y, int z) {
        const int trunkHeight = 4 + rand() % 3; // 4–6
        const int leafRadius = 2;

        // Ствол
        for (int i = 0; i < trunkHeight; ++i) {
            Voxel& vox = voxmodel->getVoxel(x, y + i, z);
            vox.setColor(glm::vec4(0.4f, 0.2f, 0.0f, 1.0f));
            vox.visible = true;
        }

        // Крона (кубическая)
        for (int dx = -leafRadius; dx <= leafRadius; ++dx) {
            for (int dy = 0; dy <= leafRadius; ++dy) {
                for (int dz = -leafRadius; dz <= leafRadius; ++dz) {
                    int dist2 = dx*dx + dy*dy + dz*dz;
                    if (dist2 <= leafRadius*leafRadius) {
                        Voxel& leaf = voxmodel->getVoxel(x + dx, y + trunkHeight + dy, z + dz);
                        leaf.setColor(glm::vec4(0.0f, 0.8f, 0.0f, 1.0f));
                        leaf.visible = true;
                    }
                }
            }
        }
    }

    VoxelStructure* generateMap() {
        // VoxelStructure* voxmodel = new VoxelStructure();

        // FastNoiseLite noise;
        // noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
        // noise.SetSeed(dist(gen));
        // noise.SetFrequency(0.01f);
        // noise.SetFractalOctaves(4);
        // noise.SetFractalLacunarity(2.0f);
        // noise.SetFractalGain(0.5f);

        // // Дополнительный генератор для мелкой шероховатости
        // FastNoiseLite detailNoise;
        // detailNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
        // detailNoise.SetSeed(dist(gen));
        // detailNoise.SetFrequency(0.05f);
        // detailNoise.SetFractalOctaves(2);

        // const int maxHeight = _size.y; // максимальная высота карты
        // const int waterLevel = maxHeight / 3; // уровень воды
        // const int hillLevel = 2 * maxHeight / 3; // уровень холмов

        // // Случайные холмы
        // struct Hill { int x, z, radius, height; };
        // std::vector<Hill> hills;
        // for (int i = 0; i < 10; ++i) { // 10 случайных холмов
        //     hills.push_back({rand() % _size.x - _size.x/2,
        //                     rand() % _size.z - _size.z/2,
        //                     5 + rand() % 10,
        //                     5 + rand() % 15});
        // }

        // glm::vec4 color;

        // for (int x = -_size.x / 2; x < _size.x / 2; ++x) {
        //     for (int z = -_size.z / 2; z < _size.z / 2; ++z) {
        //         float n = noise.GetNoise(float(x), float(z)); // [-1,1]
        //         n = (n + 1.0f) / 2.0f; // [0,1]

        //         // Мелкая шероховатость
        //         float detail = detailNoise.GetNoise(float(x), float(z));
        //         detail = (detail + 1.0f) / 2.0f;

        //         // Холмы
        //         float hillFactor = 0.0f;
        //         for (auto& hill : hills) {
        //             float dx = x - hill.x;
        //             float dz = z - hill.z;
        //             float dist2 = dx*dx + dz*dz;
        //             hillFactor += std::exp(-dist2 / (2.0f * hill.radius * hill.radius)) * hill.height;
        //         }

        //         int height = int(n * maxHeight + detail * 3.0f + hillFactor);
        //         height = std::clamp(height, 0, maxHeight - 1);

        //         // Цвет в зависимости от высоты
        //         if (height < waterLevel) {
        //             color = glm::vec4(0.0f, 0.3f, 0.8f, 0.5f);
        //         } else if (height < hillLevel) {
        //             float t = float(height - waterLevel) / float(hillLevel - waterLevel);
        //             glm::vec3 baseColor = glm::vec3(0.2f, 0.8f, 0.2f);
        //             glm::vec3 finalColor = baseColor * (0.5f + 0.5f * t);
        //             color = glm::vec4(finalColor, 1.0f);
        //         } else {
        //             float t = float(height - hillLevel) / float(maxHeight - hillLevel);
        //             glm::vec3 baseColor = glm::vec3(0.5f, 0.5f, 0.5f);
        //             glm::vec3 finalColor = glm::mix(baseColor, glm::vec3(1.0f,1.0f,1.0f), t);
        //             color = glm::vec4(finalColor, 1.0f);
        //         }

        //         int ix = x + _size.x / 2;
        //         int iz = z + _size.z / 2;
        //         _heightarray[iz * _size.x + ix] = height;

        //         Voxel& vox = voxmodel->getVoxel(x, height, z);
        //         vox.setColor(color);
        //         vox.visible = true;

        //         // Случайные деревья
        //         if (height > hillLevel + 2 && (rand() % 1000) < 3) {
        //             createTree(voxmodel, x, height, z);
        //         }
        //     }
        // }

        // return voxmodel;
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