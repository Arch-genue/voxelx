#pragma once

#include "stdlib.h"

class Mesh;
class VoxModel;

class ModelRenderer {
	float* buffer;
	size_t capacity;
public:
	ModelRenderer(size_t capacity);
	~ModelRenderer();

	Mesh* render(VoxModel* chunk);
};