#include "renderer.h"
#include "mesh.h"
#include "textmesh.h"
#include "../voxels/particlesmodel.h"
#include <glm/glm.hpp>

#include <iostream>
#include "../utils.h"

//TODO Пошаговый рендеринг мешей

#define IS_IN(voxels, X,Y,Z) ((X) >= 0 && (X) < voxels->getSize().x && (Y) >= 0 && (Y) < voxels->getSize().y && (Z) >= 0 && (Z) < voxels->getSize().z)

#define IS_BLOCKED(voxels, X,Y,Z) (IS_IN(voxels, X, Y, Z) && voxels->getVoxel(glm::vec3(X,Y,Z)))

#define VERTEX_SIZE (3 + 3 + 3 + 4)

int chunk_attrs[] = {3,3,3,4, 0};

float* Renderer::buffer;
size_t Renderer::capacity;

Camera* Renderer::camera;

size_t Renderer::_index;

void Renderer::init(size_t capacity) {
	buffer = new float[capacity * VERTEX_SIZE * 6];
	Renderer::capacity = capacity;

	errorprint("RENDERER", "Renderer initialized",  MSGINFO);
	errorprint("RENDERER", "Max render size: " + std::string(BLUE_COLOR) + std::to_string(Renderer::capacity) + std::string(RESET_COLOR),  MSGINFO);
}

void Renderer::free() {
	delete[] buffer;

}
void Renderer::addCamera(Camera* cam) {
	camera = cam;
}

Mesh* Renderer::render(ParticlesModel* voxels) {
	_index = 0;
	
	std::string renderside = voxels->getRenderSide();

	for (size_t i = 0; i < voxels->getVoxelsCount(); i++) {
		Voxel* voxel = voxels->getVoxel(i);

		computeVoxelRender(voxels, voxel, renderside);
	}
	return new Mesh(voxels, buffer, _index / VERTEX_SIZE, chunk_attrs);
}

Mesh* Renderer::render(VoxelModel* voxels) {
	_index = 0;
	
	std::string renderside = voxels->getRenderSide();

	auto start = std::chrono::high_resolution_clock::now();
	for (size_t x = 0; x < voxels->getSize().x; x++) {
		for (size_t y = 0; y < voxels->getSize().y; y++) {
			for (size_t z = 0; z < voxels->getSize().z; z++) {
				Voxel* voxel = voxels->getVoxel(glm::ivec3(x, y, z));
				computeVoxelRender(voxels, voxel, renderside);
			}
		}
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = end - start;

	errorprint("RENDERER", "GENERATED MESH: " + std::string(CYAN_COLOR) + voxels->getName() + "	" + std::string(BLUE_COLOR) + std::to_string(duration.count()) + "s" + std::string(RESET_COLOR),  MSGINFO);
	return new Mesh(voxels, buffer, _index / VERTEX_SIZE, chunk_attrs);
}
void Renderer::computeVoxelRender(VoxelModel* voxels, Voxel* voxel, std::string renderside) {
	if (voxel == nullptr) return;
	if (!voxel->isVisible()) return;

	float x = voxel->getPosition().x;
	float y = voxel->getPosition().y;
	float z = voxel->getPosition().z;
	
	glm::vec4 clr = voxel->getColor();
	
	//? Y
	if (!IS_BLOCKED(voxels, x,y+1,z)) {
		top(_index, x, y, z, clr);
	} 
	if (!IS_BLOCKED(voxels, x,y-1,z)) {
		bottom(_index, x, y, z, clr);
	}

	//? X
	if (!IS_BLOCKED(voxels, x+1,y,z)) {
		left(_index, x, y, z, clr);
	}
	if (!IS_BLOCKED(voxels, x-1,y,z)) {
		right(_index, x, y, z, clr);
	}

	//? Z
	if (!IS_BLOCKED(voxels, x,y,z+1)) {
		front(_index, x, y, z, clr);
	}
	if (!IS_BLOCKED(voxels, x,y,z-1)) {
		back(_index, x, y, z, clr);
	}
}

void Renderer::vertex(float x, float y, float z, float vert_x, float vert_y, float vert_z, glm::vec3 &normal, glm::vec4 clr) {
	buffer[_index]   = x + vert_x;
	buffer[_index+1] = y + vert_y;
	buffer[_index+2] = z + vert_z;

	buffer[_index+3] = vert_x;
	buffer[_index+4] = vert_y;
	buffer[_index+5] = vert_z;
	
	buffer[_index+6] = normal.x;
	buffer[_index+7] = normal.y;
	buffer[_index+8] = normal.z;

	buffer[_index+9] = clr.x;
	buffer[_index+10] = clr.y;
	buffer[_index+11] = clr.z;
	buffer[_index+12] = clr.w;

	_index += VERTEX_SIZE;
}

void Renderer::top(size_t &index, float x, float y, float z, glm::vec4 clr) {
	glm::vec3 normal(0.0f, 1.0f, 0.0f);
	
	vertex(x,y,z, -0.5f, +0.5f, -0.5f, normal, clr);
	vertex(x,y,z, -0.5f, +0.5f, +0.5f, normal, clr);
	vertex(x,y,z, +0.5f, +0.5f, +0.5f, normal, clr);
	vertex(x,y,z, -0.5f, +0.5f, -0.5f, normal, clr);
	vertex(x,y,z, +0.5f, +0.5f, +0.5f, normal, clr);
	vertex(x,y,z, +0.5f, +0.5f, -0.5f, normal, clr);
}
void Renderer::bottom(size_t &index, float x, float y, float z, glm::vec4 clr) {
	glm::vec3 normal(0.0f, -1.0f, 0.0f);

	vertex(x,y,z, -0.5f, -0.5f, -0.5f, normal, clr);
	vertex(x,y,z, +0.5f, -0.5f, +0.5f, normal, clr);
	vertex(x,y,z, -0.5f, -0.5f, +0.5f, normal, clr);
	vertex(x,y,z, -0.5f, -0.5f, -0.5f, normal, clr);
	vertex(x,y,z, +0.5f, -0.5f, -0.5f, normal, clr);
	vertex(x,y,z, +0.5f, -0.5f, +0.5f, normal, clr);
}
void Renderer::left(size_t &index, float x, float y, float z, glm::vec4 clr) {
	glm::vec3 normal(1.0f, 0.0f, 0.0f);

	vertex(x,y,z, +0.5f, -0.5f, -0.5f, normal, clr);
	vertex(x,y,z, +0.5f, +0.5f, -0.5f, normal, clr);
	vertex(x,y,z, +0.5f, +0.5f, +0.5f, normal, clr);

	vertex(x,y,z, +0.5f, -0.5f, -0.5f, normal, clr);
	vertex(x,y,z, +0.5f, +0.5f, +0.5f, normal, clr);
	vertex(x,y,z, +0.5f, -0.5f, +0.5f, normal, clr);
}
void Renderer::right(size_t &index, float x, float y, float z, glm::vec4 clr) {
	glm::vec3 normal(-1.0f, 0.0f, 0.0f);

	vertex(x,y,z, -0.5f, -0.5f, -0.5f, normal, clr);
	vertex(x,y,z, -0.5f, +0.5f, +0.5f, normal, clr);
	vertex(x,y,z, -0.5f, +0.5f, -0.5f, normal, clr);
	vertex(x,y,z, -0.5f, -0.5f, -0.5f, normal, clr);
	vertex(x,y,z, -0.5f, -0.5f, +0.5f, normal, clr);
	vertex(x,y,z, -0.5f, +0.5f, +0.5f, normal, clr);
}
void Renderer::front(size_t &index, float x, float y, float z, glm::vec4 clr) {
	glm::vec3 normal(0.0f, 0.0f, 1.0f);

	vertex(x,y,z, -0.5f, -0.5f, +0.5f, normal, clr);
	vertex(x,y,z, +0.5f, +0.5f, +0.5f, normal, clr);
	vertex(x,y,z, -0.5f, +0.5f, +0.5f, normal, clr);
	vertex(x,y,z, -0.5f, -0.5f, +0.5f, normal, clr);
	vertex(x,y,z, +0.5f, -0.5f, +0.5f, normal, clr);
	vertex(x,y,z, +0.5f, +0.5f, +0.5f, normal, clr);
}
void Renderer::back(size_t &index, float x, float y, float z, glm::vec4 clr) {
	glm::vec3 normal(0.0f, 0.0f, -1.0f);

	vertex(x,y,z, -0.5f, -0.5f, -0.5f, normal, clr);
	vertex(x,y,z, -0.5f, +0.5f, -0.5f, normal, clr);
	vertex(x,y,z, +0.5f, +0.5f, -0.5f, normal, clr);
	vertex(x,y,z, -0.5f, -0.5f, -0.5f, normal, clr);
	vertex(x,y,z, +0.5f, +0.5f, -0.5f, normal, clr);
	vertex(x,y,z, +0.5f, -0.5f, -0.5f, normal, clr);
}

Camera* Renderer::getCamera() {
	return camera;
}