#pragma once

#include "stdlib.h"

class Mesh;
class Shader;

class _voxels;
class voxel_m;

class Renderer {
	float* buffer;
	size_t capacity;

	//? GLOBAL ARRAYS
	int _shaderindex;
	int _meshindex;
	Shader** shaders;
	Mesh** meshes;
public:
	Renderer(size_t capacity, int shaderscount, int meshescount);
	~Renderer();

	void addShader(Shader* shader);
	void addMesh(Mesh* mesh);

	Mesh* render(_voxels* voxels=nullptr);
	Mesh* voxelRender(voxel_m* voxel);
};