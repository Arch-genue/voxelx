#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <map>

#include "../gamesystems/camera.h"
#include <GL/glew.h>

class Mesh;
class Camera;

class VoxelModel;
class ParticlesModel;
class Voxel;

class Renderer {
	static float* buffer;
	static size_t capacity;

	static Camera* camera;

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

	static void computeVoxelRender(size_t& index, VoxelModel* voxels, Voxel* voxel, std::string renderside);

	static Camera* getCamera();
};