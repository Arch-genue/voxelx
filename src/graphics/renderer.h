#pragma once

#include "stdlib.h"
#include <string>
#include <map>

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

	std::map<std::string, _voxels*> rowmodels;
	
	// _voxels** rowmodels;
	// std::string modelnames;
public:
	Renderer(size_t capacity, int shaderscount, int meshescount);
	~Renderer();

	void addShader(Shader* shader);
	void addRowModel(std::string name, _voxels* row);
	void addMesh(Mesh* mesh);

	Mesh* render(_voxels* voxels=nullptr);
	Mesh* voxelRender(voxel_m* voxel);

	Shader* getDefaultShader();
	_voxels* getRowModel(const char* model);
};