#pragma once

#include "stdlib.h"

class Mesh;
class _voxels;
class voxel_m;

class VoxelRenderer {
	float* buffer;
	size_t capacity;
public:
	VoxelRenderer(size_t capacity);
	~VoxelRenderer();

	Mesh* render(_voxels* voxels=nullptr);
	Mesh* voxelRender(voxel_m* voxel);
};