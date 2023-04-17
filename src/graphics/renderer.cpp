#include "renderer.h"
#include "mesh.h"
#include "../voxels/voxel.h"
#include <glm/glm.hpp>

#include <iostream>

#define VERTEX_SIZE (3 + 4 + 1)
int chunk_attrs[] = {3,4,1, 0};

#define IS_IN(X,Y,Z) ((X) >= 0 && (X) < voxels->m_size.x && (Y) >= 0 && (Y) < voxels->m_size.y && (Z) >= 0 && (Z) < voxels->m_size.z)
#define VOXEL(X,Y,Z) (voxels->voxels[((Y) * (int)voxels->m_size.z + (Z)) * (int)voxels->m_size.x + (X)])
#define IS_BLOCKED(X,Y,Z) (false)//((IS_IN(X, Y, Z)) && VOXEL(X, Y, Z).visible)

#define VERTEX(INDEX, X,Y,Z, CLR,L) \
					buffer[INDEX+0] = (X); \
					buffer[INDEX+1] = (Y); \
					buffer[INDEX+2] = (Z); \
					buffer[INDEX+3] = (CLR).x;\
					buffer[INDEX+4] = (CLR).y;\
					buffer[INDEX+5] = (CLR).z;\
					buffer[INDEX+6] = (CLR).w;\
					buffer[INDEX+7] = (L);\
					INDEX += VERTEX_SIZE;

Renderer::Renderer(size_t capacity, int shaderscount, int meshescount) : capacity(capacity) {
	buffer = new float[capacity * VERTEX_SIZE * 6];
	shaders = new Shader*[shaderscount];
	meshes = new Mesh*[meshescount];

	_shaderindex = 0;
}

Renderer::~Renderer() {
	delete[] buffer;
	delete[] shaders;
	delete[] meshes;
}

void Renderer::addShader(Shader* shader) {
	shaders[_shaderindex] = shader;
	_shaderindex++;
}
void Renderer::addRowModel(std::string name, _voxels* row) {
	rowmodels[name] = row;
}

void Renderer::addMesh(Mesh* mesh) {
	meshes[_meshindex] = mesh;
	_meshindex++;
}
void Renderer::addCamera(Camera* cam) {
	camera = cam;
}

Mesh* Renderer::render(_voxels* voxels) {
	size_t index = 0;
	float x, y, z, l;
	glm::vec4 clr;
	bool side[6]; //! top, bottom, left, right, front, back
	
	for(uint8_t i = 0; i < 6; i++) side[i] = false;

	for (size_t i = 0; i < voxels->voxels.size(); i++) {
		if (!voxels->voxels[i].visible) continue;

		x = voxels->voxels[i].position.x;
		y = voxels->voxels[i].position.y;
		z = voxels->voxels[i].position.z;
		clr = voxels->voxels[i].clr;

		if (voxels->renderSide == "") {
			for(uint8_t i = 0; i < 6; i++) side[i] = true;
		} else if(voxels->renderSide == "top") {
			side[0] = true;
		}

		//std::cout << voxels->renderSide << std::endl;
		
		//? Y
		if (!IS_BLOCKED(x,y+1,z) && side[0] == true) {
			l = 1.0f;
			VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, clr,l);
			VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, clr,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, clr,l);

			VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, clr,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, clr,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, clr,l);
		} 
		if (!IS_BLOCKED(x,y-1,z) && side[1] == true) {
			l = 0.75f;
			VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, clr,l);
			VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, clr,l);
			VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, clr,l);

			VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, clr,l);
			VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, clr,l);
			VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, clr,l);
		}

		//? X
		if (!IS_BLOCKED(x+1,y,z) && side[2] == true) {
			l = 0.95f;
			VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, clr,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, clr,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, clr,l);

			VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, clr,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, clr,l);
			VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, clr,l);
		}
		if (!IS_BLOCKED(x-1,y,z) && side[3] == true) {
			l = 0.85f;
			VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, clr,l);
			VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, clr,l);
			VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, clr,l);

			VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, clr,l);
			VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, clr,l);
			VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, clr,l);
		}

		//? Z
		if (!IS_BLOCKED(x,y,z+1) && side[4] == true) {
			l = 0.9f;
			VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, clr,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, clr,l);
			VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, clr,l);

			VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, clr,l);
			VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, clr,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, clr,l);
		}
		if (!IS_BLOCKED(x,y,z-1) && side[5] == true) {
			l = 0.8f;
			VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, clr,l);
			VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, clr,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, clr,l);

			VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, clr,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, clr,l);
			VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, clr,l);
		}
	}
	return new Mesh(voxels, buffer, index / VERTEX_SIZE, chunk_attrs);
}

Shader* Renderer::getDefaultShader() {
	return shaders[0];		
}
Shader* Renderer::getBBOXShader() {
	return shaders[2];		
}
_voxels* Renderer::getRowModel(const char* model) {
	return rowmodels[model];
}
Camera* Renderer::getCamera() {
	return camera;
}