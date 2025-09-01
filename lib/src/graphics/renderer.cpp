#include "renderer.h"
#include "meshmodel.h"
#include <glm/glm.hpp>
#include <GL/glew.h>

#include <iostream>
#include "../utilities/logger.hpp"

Camera* Renderer::camera;
constexpr float asize = 0.5f;

void Renderer::addCamera(Camera* cam) { camera = cam; }
Camera* Renderer::getCamera() { return camera; }

struct Vertex {
    glm::vec3 pos;
    glm::ivec3 normal;
    Color& color;

    bool operator==(const Vertex& other) const {
        return pos == other.pos && normal == other.normal &&
			color.r==other.color.r && 
			color.g==other.color.g &&
			color.b==other.color.b && 
			color.a==other.color.a;
    }
};

MeshModel* Renderer::generateMeshModel(VoxelModel *voxelmodel) {
	MeshModel* meshmodel = new MeshModel(voxelmodel);
	voxelmodel->forEachChunk([&] (VoxelModel::ChunkType &chunk, ChunkCoord chunkpos) {
		meshmodel->set(&chunk, generateMesh(voxelmodel, chunk));
	});

	return meshmodel;
}

Mesh* Renderer::generateMesh(VoxelModel* voxelmodel, VoxelModel::ChunkType& chunk) {
	std::vector <float> posBuffer;
	std::vector <int8_t> normalBuffer;
	std::vector <uint8_t> colorBuffer;

	constexpr size_t stride = sizeof(float) * 3 + sizeof(int8_t) * 3 + sizeof(uint8_t) * 4;

	posBuffer.reserve(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 36 * 3);
	normalBuffer.reserve(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 36 * 3);
	colorBuffer.reserve(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 36 * 4);
	
	// auto start = std::chrono::high_resolution_clock::now();
	// voxelmodel->forEachVisibleVoxelInChunk(chunk, [&] (Voxel &voxel, int x, int y, int z) {
	// 	Color& clr = voxel.getColor();
	// 	if (!voxelmodel->getVoxel(x, y+1, z).visible) top(posBuffer, normalBuffer, colorBuffer, x, y, z, clr);
	// 	if (!voxelmodel->getVoxel(x, y-1, z).visible) bottom(posBuffer, normalBuffer, colorBuffer, x, y, z, clr);

	// 	if (!voxelmodel->getVoxel(x+1, y, z).visible) left(posBuffer, normalBuffer, colorBuffer, x, y, z, clr);
	// 	if (!voxelmodel->getVoxel(x-1, y, z).visible) right(posBuffer, normalBuffer, colorBuffer, x, y, z, clr);

	// 	if (!voxelmodel->getVoxel(x, y, z+1).visible) front(posBuffer, normalBuffer, colorBuffer, x, y, z, clr);
	// 	if (!voxelmodel->getVoxel(x, y, z-1).visible) back(posBuffer, normalBuffer, colorBuffer, x, y, z, clr);
	// });

	// auto end = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<float> duration = end - start;
	// std::cout << "Legacy variant: " << duration.count() << "s \n";

	// start = std::chrono::high_resolution_clock::now();
	voxelmodel->forEachVisibleVoxelInChunk(chunk, [&] (Voxel &voxel, int x, int y, int z) {
		Color& clr = voxel.getColor();
		// chunk.getVoxel().visible;

		if (!chunk.getVoxel(x, y+1, z).visible) top(posBuffer, normalBuffer, colorBuffer, x, y, z, clr);
		if (!chunk.getVoxel(x, y-1, z).visible) bottom(posBuffer, normalBuffer, colorBuffer, x, y, z, clr);

		if (!chunk.getVoxel(x+1, y, z).visible) left(posBuffer, normalBuffer, colorBuffer, x, y, z, clr);
		if (!chunk.getVoxel(x-1, y, z).visible) right(posBuffer, normalBuffer, colorBuffer, x, y, z, clr);

		if (!chunk.getVoxel(x, y, z+1).visible) front(posBuffer, normalBuffer, colorBuffer, x, y, z, clr);
		if (!chunk.getVoxel(x, y, z-1).visible) back(posBuffer, normalBuffer, colorBuffer, x, y, z, clr);
	});
	// end = std::chrono::high_resolution_clock::now();
    // duration = end - start;
	// std::cout << "New variant: " << duration.count() << "s \n";
	// std::exit(1);

	Mesh* mesh = new Mesh(posBuffer, normalBuffer, colorBuffer);
	mesh->_vertices = posBuffer.size() / 3; // по количеству float в позиции

	glBindVertexArray(mesh->_getvao());

	// Позиции
	glBindBuffer(GL_ARRAY_BUFFER, mesh->_get_pos_vbo());
	glBufferData(GL_ARRAY_BUFFER, posBuffer.size() * sizeof(float), posBuffer.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// Нормали
	glBindBuffer(GL_ARRAY_BUFFER, mesh->_get_normal_vbo());
	glBufferData(GL_ARRAY_BUFFER, normalBuffer.size() * sizeof(int8_t), normalBuffer.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_BYTE, GL_TRUE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	// Цвет
	glBindBuffer(GL_ARRAY_BUFFER, mesh->_get_color_vbo());
	glBufferData(GL_ARRAY_BUFFER, colorBuffer.size() * sizeof(uint8_t), colorBuffer.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);
	glEnableVertexAttribArray(2);

    glBindVertexArray(0);
	
	return mesh;
}

void Renderer::vertex(
	std::vector<float>& posBuffer,
	std::vector<int8_t>& normalBuffer,
	std::vector<uint8_t>& colorBuffer,
	float x, float y, float z, 
	const glm::ivec3 &normal, 
	Color& clr
) {
	
	posBuffer.push_back(x);
	posBuffer.push_back(y);
	posBuffer.push_back(z);

	normalBuffer.push_back(normal.x);
	normalBuffer.push_back(normal.y);
	normalBuffer.push_back(normal.z);

	colorBuffer.push_back(clr.r);
	colorBuffer.push_back(clr.g);
	colorBuffer.push_back(clr.b);
	colorBuffer.push_back(clr.a);
}

void Renderer::top(
	std::vector<float>& posBuffer,
	std::vector<int8_t>& normalBuffer,
	std::vector<uint8_t>& colorBuffer, 
	float x, float y, float z, Color& clr
) {
	const glm::ivec3 normal(0.0f, 1.0f, 0.0f);
	
	vertex(posBuffer, normalBuffer, colorBuffer, x-asize, y+asize, z-asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x-asize, y+asize, z+asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x+asize, y+asize, z+asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x-asize, y+asize, z-asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x+asize, y+asize, z+asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x+asize, y+asize, z-asize, normal, clr);
}

void Renderer::bottom(
	std::vector<float>& posBuffer,
	std::vector<int8_t>& normalBuffer,
	std::vector<uint8_t>& colorBuffer, 
	float x, float y, float z, Color& clr
) {
	const glm::ivec3 normal(0.0f, -1.0f, 0.0f);

	vertex(posBuffer, normalBuffer, colorBuffer, x-asize, y-asize, z-asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x+asize, y-asize, z+asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x-asize, y-asize, z+asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x-asize, y-asize, z-asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x+asize, y-asize, z-asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x+asize, y-asize, z+asize, normal, clr);
}
void Renderer::left(
	std::vector<float>& posBuffer,
	std::vector<int8_t>& normalBuffer,
	std::vector<uint8_t>& colorBuffer, 
	float x, float y, float z, Color& clr
) {
	const glm::ivec3 normal(1.0f, 0.0f, 0.0f);

	vertex(posBuffer, normalBuffer, colorBuffer, x+asize, y-asize, z-asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x+asize, y+asize, z-asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x+asize, y+asize, z+asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x+asize, y-asize, z-asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x+asize, y+asize, z+asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x+asize, y-asize, z+asize, normal, clr);
}
void Renderer::right(
	std::vector<float>& posBuffer,
	std::vector<int8_t>& normalBuffer,
	std::vector<uint8_t>& colorBuffer, 
	float x, float y, float z, Color& clr
) {	
	const glm::ivec3 normal(-1.0f, 0.0f, 0.0f);

	vertex(posBuffer, normalBuffer, colorBuffer, x-asize, y-asize, z-asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x-asize, y+asize, z+asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x-asize, y+asize, z-asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x-asize, y-asize, z-asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x-asize, y-asize, z+asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x-asize, y+asize, z+asize, normal, clr);
}
void Renderer::front(
	std::vector<float>& posBuffer,
	std::vector<int8_t>& normalBuffer,
	std::vector<uint8_t>& colorBuffer, 
	float x, float y, float z, Color& clr
) {	
	const glm::ivec3 normal(0.0f, 0.0f, 1.0f);

	vertex(posBuffer, normalBuffer, colorBuffer, x-asize, y-asize, z+asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x+asize, y-asize, z+asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x+asize, y+asize, z+asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x-asize, y-asize, z+asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x+asize, y+asize, z+asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x-asize, y+asize, z+asize, normal, clr);
}
void Renderer::back(
	std::vector<float>& posBuffer,
	std::vector<int8_t>& normalBuffer,
	std::vector<uint8_t>& colorBuffer, 
	float x, float y, float z, Color& clr
) {
		const glm::ivec3 normal(0.0f, 0.0f, -1.0f);

	vertex(posBuffer, normalBuffer, colorBuffer, x-asize, y-asize, z-asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x-asize, y+asize, z-asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x+asize, y+asize, z-asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x-asize, y-asize, z-asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x+asize, y+asize, z-asize, normal, clr);
	vertex(posBuffer, normalBuffer, colorBuffer, x+asize, y-asize, z-asize, normal, clr);
}