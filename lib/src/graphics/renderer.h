/**
 * @file renderer.h
 * @author Vlad Kartsaev
 * @brief Voxel generation functions
 * @version 0.8
 * @date 2023-04-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "voxels/voxelstructure.h"

class Renderer {
public:
    static void generate_mesh(VoxelStructure *structure, VoxelChunk& chunk, size_t& vertices, GLuint& vao, GLuint& vboPos, GLuint& vboNormal, GLuint& vboColor);

private:
    Renderer() = default;
    ~Renderer() = default;

    static void quadFace(
        const glm::vec3& v0,
        const glm::vec3& v1,
        const glm::vec3& v2,
        const glm::vec3& v3,
        const glm::ivec3& normal,
        const Color& clr
    );
    static void vertex(
        const glm::vec3 &pos, 
        const glm::ivec3 &normal, 
        const Color& clr
    );

    static std::vector<float> _posBuffer;
    static std::vector<int8_t> _normalBuffer;
    static std::vector<uint8_t> _colorBuffer;

};