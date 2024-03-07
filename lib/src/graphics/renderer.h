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

#include <stdlib.h>
#include <stdint.h>
#include <string>

#include <GL/glew.h>

#include "../gamesystems/camera.h"

class Mesh;
class Camera;

class VoxelModel;
class ParticlesModel;
class Voxel;

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec4 color;
};

class Renderer {
	static float* buffer;
	static size_t capacity;

	static Camera* camera;

	static size_t _index;

	static void vertex(float x, float y, float z, float vert_x, float vert_y, float vert_z, glm::vec3 &normal, glm::vec4 clr);

	static void top(size_t &index, float x, float y, float z, glm::vec4 clr);
	static void bottom(size_t &index, float x, float y, float z, glm::vec4 clr);
	static void left(size_t &index, float x, float y, float z, glm::vec4 clr);
	static void right(size_t &index, float x, float y, float z, glm::vec4 clr);
	static void front(size_t &index, float x, float y, float z, glm::vec4 clr);
	static void back(size_t &index, float x, float y, float z, glm::vec4 clr);
public:
	static void init(size_t capacity);
    static void free();

    static void addCamera(Camera* cam);

	static Mesh* render(VoxelModel* voxels=nullptr);
	static Mesh* render(ParticlesModel* voxels=nullptr);

	static void computeVoxelRender(VoxelModel* voxels, Voxel* voxel, std::string renderside);

    static Camera* getCamera();
};