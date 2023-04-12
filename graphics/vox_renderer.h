#pragma once

#include "stdlib.h"

class Mesh;
//class Model;
class _voxels;
class voxel_m;

class ModelRenderer {
	float* buffer;
	size_t capacity;
public:
	ModelRenderer(size_t capacity);
	~ModelRenderer();

	Mesh* render(_voxels* voxels=nullptr);
	Mesh* voxelRender(voxel_m* voxel);
};