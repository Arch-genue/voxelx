#pragma once

#include "stdlib.h"
#include <string>
#include <map>
#include "../gamesystems/camera.h"

class Mesh;
class Shader;
class Camera;

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

	Camera* camera;
public:
	Renderer(size_t capacity, int shaderscount, int meshescount);
	~Renderer();

	void addShader(Shader* shader);
	void addRowModel(std::string name, _voxels* row);
	void addMesh(Mesh* mesh);
	void addCamera(Camera* cam);

	Mesh* render(_voxels* voxels=nullptr);
	Mesh* voxelRender(voxel_m* voxel);

	Shader* getDefaultShader();
	Shader* getBBOXShader();
	Shader* getUIShader();
	Shader* getCrosshairShader();
	_voxels* getRowModel(const char* model);
	Camera* getCamera();
};