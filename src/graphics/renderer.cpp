#include "renderer.h"
#include "mesh.h"
#include "textmesh.h"
#include "../voxels/particlesmodel.h"
#include <glm/glm.hpp>

#include <iostream>
#include "../utils.h"

// class Shader;

//TODO Пошаговый рендеринг мешей

#define IS_IN(voxels, X,Y,Z) ((X) >= 0 && (X) < voxels->getSize().x && (Y) >= 0 && (Y) < voxels->getSize().y && (Z) >= 0 && (Z) < voxels->getSize().z)

#define IS_BLOCKED(voxels, X,Y,Z) (IS_IN(voxels, X, Y, Z) && voxels->getVoxel(glm::vec3(X,Y,Z)))

#define VERTEX(INDEX, X,Y,Z, CLR) \
					buffer[INDEX+0] = (X); \
					buffer[INDEX+1] = (Y); \
					buffer[INDEX+2] = (Z); \
					buffer[INDEX+3] = (CLR).x;\
					buffer[INDEX+4] = (CLR).y;\
					buffer[INDEX+5] = (CLR).z;\
					buffer[INDEX+6] = (CLR).w;\
					INDEX += VERTEX_SIZE;

#define VERTEX_SIZE (3 + 4)

int chunk_attrs[] = {3,4, 0};


float* Renderer::buffer;
size_t Renderer::capacity;

Camera* Renderer::camera;

void Renderer::init(size_t capacity) {
	buffer = new float[capacity * VERTEX_SIZE * 6];
	Renderer::capacity = capacity;

	errorprint("RENDERER", "Renderer initialized",  MSGSUCCESS);
	errorprint("RENDERER", "Max render size: " + std::string(BLUE_COLOR) + std::to_string(Renderer::capacity) + std::string(RESET_COLOR),  MSGSUCCESS);
}

void Renderer::free() {
	delete[] buffer;

}
void Renderer::addCamera(Camera* cam) {
	camera = cam;
}

Mesh* Renderer::render(ParticlesModel* voxels) {
	size_t index {0};
	
	std::string renderside = voxels->getRenderSide();

	for (size_t i = 0; i < voxels->getVoxelsCount(); i++) {
		Voxel* voxel = voxels->getVoxel(i);

		computeVoxelRender(index, voxels, voxel, renderside);
	}
	return new Mesh(voxels, buffer, index / VERTEX_SIZE, chunk_attrs);
}

Mesh* Renderer::render(VoxelModel* voxels) {
	size_t index {0};
	
	std::string renderside = voxels->getRenderSide();
	int i = 0;
	auto start = std::chrono::high_resolution_clock::now();
	for (size_t x = 0; x < voxels->getSize().x; x++) {
		for (size_t y = 0; y < voxels->getSize().y; y++) {
			for (size_t z = 0; z < voxels->getSize().z; z++) {
				// auto start = std::chrono::steady_clock::now(); // Засекаем начало времени
				Voxel* voxel = voxels->getVoxel(glm::ivec3(x, y, z));

				// auto end = std::chrono::steady_clock::now(); // Засекаем конец времени

				// // Вычисляем время выполнения в секундах
				// float duration = std::chrono::duration<float>(end - start).count();
				// std::cout << i << " " << "Time taken: " << duration << " seconds" << std::endl;

				computeVoxelRender(index, voxels, voxel, renderside);
				// float time = measureFunctionTime(computeVoxelRender, index, voxels, voxel, renderside);
				// std::cout << i << " " << "Time taken: " << time << " seconds" << std::endl;
				i++;
			}
		}
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = end - start;

	errorprint("RENDERER", "GENERATED MESH: " + std::string(CYAN_COLOR) + voxels->getName() + "	" + std::string(BLUE_COLOR) + std::to_string(duration.count()) + "s" + std::string(RESET_COLOR),  MSGINFO);
	return new Mesh(voxels, buffer, index / VERTEX_SIZE, chunk_attrs);
}
void Renderer::computeVoxelRender(size_t& index, VoxelModel* voxels, Voxel* voxel, std::string renderside) {
	if (voxel == nullptr) return;
	if (!voxel->isVisible()) return;

	int x = voxel->getPosition().x;
	int y = voxel->getPosition().y;
	int z = voxel->getPosition().z;
	
	glm::vec4 clr = voxel->getColor();
	
	//? Y
	if (!IS_BLOCKED(voxels, x,y+1,z)) {
		top(index, x, y, z, clr);
	} 
	if (!IS_BLOCKED(voxels, x,y-1,z)) {
		bottom(index, x, y, z, clr);
	}

	//? X
	if (!IS_BLOCKED(voxels, x+1,y,z)) {
		left(index, x, y, z, clr);
	}
	if (!IS_BLOCKED(voxels, x-1,y,z)) {
		right(index, x, y, z, clr);
	}

	//? Z
	if (!IS_BLOCKED(voxels, x,y,z+1)) {
		front(index, x, y, z, clr);
	}
	if (!IS_BLOCKED(voxels, x,y,z-1)) {
		back(index, x, y, z, clr);
	}
}

void Renderer::top(size_t &index, float x, float y, float z, glm::vec4 clr) {
	VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, clr);
	VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, clr);
	VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, clr);

	VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, clr);
	VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, clr);
	VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, clr);
}
void Renderer::bottom(size_t &index, float x, float y, float z, glm::vec4 clr) {
	VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, clr);
	VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, clr);
	VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, clr);

	VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, clr);
	VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, clr);
	VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, clr);
}
void Renderer::left(size_t &index, float x, float y, float z, glm::vec4 clr) {
	VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, clr);
	VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, clr);
	VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, clr);

	VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, clr);
	VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, clr);
	VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, clr);
}
void Renderer::right(size_t &index, float x, float y, float z, glm::vec4 clr) {
	VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, clr);
	VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, clr);
	VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, clr);

	VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, clr);
	VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, clr);
	VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, clr);
}
void Renderer::front(size_t &index, float x, float y, float z, glm::vec4 clr) {
	VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, clr);
	VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, clr);
	VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, clr);

	VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, clr);
	VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, clr);
	VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, clr);
}
void Renderer::back(size_t &index, float x, float y, float z, glm::vec4 clr) {
	VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, clr);
	VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, clr);
	VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, clr);

	VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, clr);
	VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, clr);
	VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, clr);
}

Camera* Renderer::getCamera() {
	return camera;
}