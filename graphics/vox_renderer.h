#pragma once

#include "stdlib.h"

class Mesh;
//class Model;
class _voxels;

class ModelRenderer {
	float* buffer;
	size_t capacity;
public:
	ModelRenderer(size_t capacity);
	~ModelRenderer();

	Mesh* render(_voxels* voxels);
};