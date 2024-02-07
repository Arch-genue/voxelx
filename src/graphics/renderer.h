#pragma once

#include "stdlib.h"
#include <string>
#include <map>
#include "../gamesystems/camera.h"

class Mesh;
class Camera;

class VoxelModel;
class Voxel;

class Renderer {
	static float* buffer;
	static size_t capacity;

	static Camera* camera;

	static void top(size_t &index, float x, float y, float z, glm::vec4 clr, glm::vec3 light);
	static void bottom(size_t &index, float x, float y, float z, glm::vec4 clr, glm::vec3 light);
	static void left(size_t &index, float x, float y, float z, glm::vec4 clr, glm::vec3 light);
	static void right(size_t &index, float x, float y, float z, glm::vec4 clr, glm::vec3 light);
	static void front(size_t &index, float x, float y, float z, glm::vec4 clr, glm::vec3 light);
	static void back(size_t &index, float x, float y, float z, glm::vec4 clr, glm::vec3 light);
public:
	static void init(size_t capacity);
	static void free();
	
	static void addCamera(Camera* cam);

	static Mesh* render(VoxelModel* voxels=nullptr);
	static Mesh* voxelRender(Voxel* voxel);

	static Camera* getCamera();
};