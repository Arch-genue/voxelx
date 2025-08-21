#include "renderer.h"
#include "mesh.h"
#include "textmesh.h"
#include <glm/glm.hpp>

#include <iostream>
#include "../utilities/logger.h"

//TODO Пошаговый рендеринг мешей

#define IS_IN(voxels, X,Y,Z) ((X) >= 0 && (X) < voxels->getSize().x && (Y) >= 0 && (Y) < voxels->getSize().y && (Z) >= 0 && (Z) < voxels->getSize().z)

#define IS_BLOCKED(voxels, X,Y,Z) (IS_IN(voxels, X, Y, Z) && voxels->getVoxel(glm::ivec3(X,Y,Z)))

#define VERTEX_SIZE (3 + 3 + 4)

int chunk_attrs[] = {3,3,4, 0};

float* Renderer::buffer;
size_t Renderer::capacity;

Camera* Renderer::camera;

size_t Renderer::_index;

float asize = 0.5f;

void Renderer::init(size_t capacity) {
	buffer = new float[capacity * VERTEX_SIZE * 6];
	Renderer::capacity = capacity;

	vLogger::eprint("RENDERER", "Renderer initialized",  LOGLEVEL::INFO);
	vLogger::eprint("RENDERER", "Max render size: " + std::string(BLUE_COLOR) + std::to_string(Renderer::capacity) + std::string(RESET_COLOR),  LOGLEVEL::INFO);
}

void Renderer::free() {
	delete[] buffer;

}
void Renderer::addCamera(Camera* cam) {
	camera = cam;
}

std::unique_ptr<Mesh> Renderer::render(VoxelModel* voxels) {
	_index = 0;

	auto start = std::chrono::high_resolution_clock::now();

	// for (auto& [chunkPos, chunk] : voxels->get_chunks()) {
	// 	for (auto& [voxelPos, voxel] : chunk.voxels) {
	// 		computeVoxelRender(voxels, &voxel, "");
	// 	}
	// }
	size_t ikkto = 0;
	voxels->forEachVoxel([&](Voxel* voxel){
		computeVoxelRender(voxels, voxel, "");
		ikkto++;
	});

	std::cout << "BIGOLO: " << ikkto << "\n";

	// for (auto& [pos, voxel] : voxelsMap) {
	// 	computeVoxelRender(voxels, &voxel, "");
	// }
	
	// for (int x = voxels->getMinSize().x; x < voxels->getSize().x; x++) {
	// 	for (int y = voxels->getMinSize().y; y < voxels->getSize().y; y++) {
	// 		for (int z = voxels->getMinSize().z; z < voxels->getSize().z; z++) {
	// 			Voxel* voxel = voxels->getVoxel(x, y, z);
	// 			if (voxel) {
	// 				computeVoxelRender(voxels, voxel, "");
	// 			}
	// 		}
	// 	}
	// }

	// Выделяем отдельный буфер для этого меша
    float* meshBuffer = new float[_index];
    std::memcpy(meshBuffer, buffer, _index * sizeof(float));

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = end - start;
	vLogger::eprint("RENDERER", "GENERATED MESH: " + std::string(CYAN_COLOR) + "	" + std::string(BLUE_COLOR) + std::to_string(duration.count()) + "s" + std::string(RESET_COLOR),  LOGLEVEL::INFO);
	return std::make_unique<Mesh>(meshBuffer, _index / VERTEX_SIZE, chunk_attrs);
}

void Renderer::__ensureBufferCapacity(size_t required) {
    if (required > Renderer::capacity) {
        size_t newCapacity = std::max(required, capacity * 2);
        float* newBuffer = new float[newCapacity * VERTEX_SIZE * 6];
        std::memcpy(newBuffer, buffer, _index * sizeof(float));
        delete[] buffer;
        buffer = newBuffer;
        Renderer::capacity = newCapacity;
    }
}

void Renderer::computeVoxelRender(VoxelModel* voxels, Voxel* voxel, std::string renderside) {
	if (voxel == nullptr) return;
	if (!voxel->visible) return;

	int x = voxel->position.x;
	int y = voxel->position.y;
	int z = voxel->position.z;
	
	glm::vec4 clr = voxel->color;
	
	// if (!voxels->getVoxel(glm::vec3(x, y+1, z)))
		// std::cout << "ERROR:" << std::endl;

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
	
	buffer[_index+3] = normal.x;
	buffer[_index+4] = normal.y;
	buffer[_index+5] = normal.z;

	buffer[_index+6] = clr.x;
	buffer[_index+7] = clr.y;
	buffer[_index+8] = clr.z;
	buffer[_index+9] = clr.w;

	_index += VERTEX_SIZE;
}

void Renderer::top(size_t &index, float x, float y, float z, glm::vec4 clr) {
	glm::vec3 normal(0.0f, 1.0f, 0.0f);
	
	vertex(x,y,z, -asize, +asize, -asize, normal, clr);
	vertex(x,y,z, -asize, +asize, +asize, normal, clr);
	vertex(x,y,z, +asize, +asize, +asize, normal, clr);
	vertex(x,y,z, -asize, +asize, -asize, normal, clr);
	vertex(x,y,z, +asize, +asize, +asize, normal, clr);
	vertex(x,y,z, +asize, +asize, -asize, normal, clr);
}
void Renderer::bottom(size_t &index, float x, float y, float z, glm::vec4 clr) {
	glm::vec3 normal(0.0f, -1.0f, 0.0f);

	vertex(x,y,z, -asize, -asize, -asize, normal, clr);
	vertex(x,y,z, +asize, -asize, +asize, normal, clr);
	vertex(x,y,z, -asize, -asize, +asize, normal, clr);
	vertex(x,y,z, -asize, -asize, -asize, normal, clr);
	vertex(x,y,z, +asize, -asize, -asize, normal, clr);
	vertex(x,y,z, +asize, -asize, +asize, normal, clr);
}
void Renderer::left(size_t &index, float x, float y, float z, glm::vec4 clr) {
	glm::vec3 normal(-1.0f, 0.0f, 0.0f);

	vertex(x,y,z, +asize, -asize, -asize, normal, clr);
	vertex(x,y,z, +asize, +asize, -asize, normal, clr);
	vertex(x,y,z, +asize, +asize, +asize, normal, clr);

	vertex(x,y,z, +asize, -asize, -asize, normal, clr);
	vertex(x,y,z, +asize, +asize, +asize, normal, clr);
	vertex(x,y,z, +asize, -asize, +asize, normal, clr);
}
void Renderer::right(size_t &index, float x, float y, float z, glm::vec4 clr) {
	glm::vec3 normal(1.0f, 0.0f, 0.0f);

	vertex(x,y,z, -asize, -asize, -asize, normal, clr);
	vertex(x,y,z, -asize, +asize, +asize, normal, clr);
	vertex(x,y,z, -asize, +asize, -asize, normal, clr);

	vertex(x,y,z, -asize, -asize, -asize, normal, clr);
	vertex(x,y,z, -asize, -asize, +asize, normal, clr);
	vertex(x,y,z, -asize, +asize, +asize, normal, clr);
}
void Renderer::front(size_t &index, float x, float y, float z, glm::vec4 clr) {
	glm::vec3 normal(0.0f, 0.0f, 1.0f);

	vertex(x,y,z, -asize, -asize, +asize, normal, clr);
	vertex(x,y,z, +asize, -asize, +asize, normal, clr);
	vertex(x,y,z, +asize, +asize, +asize, normal, clr);
	
	vertex(x,y,z, -asize, -asize, +asize, normal, clr);
	vertex(x,y,z, +asize, +asize, +asize, normal, clr);
	vertex(x,y,z, -asize, +asize, +asize, normal, clr);
}
void Renderer::back(size_t &index, float x, float y, float z, glm::vec4 clr) {
	glm::vec3 normal(0.0f, 0.0f, -1.0f);

	vertex(x,y,z, -asize, -asize, -asize, normal, clr);
	vertex(x,y,z, -asize, +asize, -asize, normal, clr);
	vertex(x,y,z, +asize, +asize, -asize, normal, clr);
	vertex(x,y,z, -asize, -asize, -asize, normal, clr);
	vertex(x,y,z, +asize, +asize, -asize, normal, clr);
	vertex(x,y,z, +asize, -asize, -asize, normal, clr);
}

Camera* Renderer::getCamera() {
	return camera;
}