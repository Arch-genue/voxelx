#pragma once

#include "stdlib.h"

class Mesh;
class _voxels;
class voxel_m;

class Renderer {
	float* buffer;
	size_t capacity;
public:
	Renderer(size_t capacity);
	~Renderer();

	Mesh* render(_voxels* voxels=nullptr);
	Mesh* voxelRender(voxel_m* voxel);
};