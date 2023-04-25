#pragma once

#include "stdlib.h"
#include <string>
#include <map>
#include "../gamesystems/camera.h"

class Mesh;
class Camera;

class _voxels;
class voxel_m;

class Renderer {
	static float* buffer;
	static size_t capacity;

	static Camera* camera;
public:
	static void init(size_t capacity);
	static void free();
	
	static void addCamera(Camera* cam);

	static Mesh* render(_voxels* voxels=nullptr);
	static Mesh* voxelRender(voxel_m* voxel);

	static Camera* getCamera();
};