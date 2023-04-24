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

	Camera* camera;
public:
	Renderer(size_t capacity);
	~Renderer();

	void addCamera(Camera* cam);

	Mesh* render(_voxels* voxels=nullptr);
	Mesh* voxelRender(voxel_m* voxel);

	Camera* getCamera();
};