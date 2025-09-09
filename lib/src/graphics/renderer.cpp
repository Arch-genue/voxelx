#include "renderer.h"

#include <iostream>

static constexpr glm::ivec3 normals[6] = { {0,1,0}, {0,-1,0}, {1,0,0}, {-1,0,0}, {0,0,1}, {0,0,-1} };
static constexpr glm::vec3 neighborOffset[6] = { {0,1,0}, {0,-1,0}, {1,0,0}, {-1,0,0}, {0,0,1}, {0,0,-1} };

static constexpr glm::vec3 offsets[6][4] = {
	{{-0.5f, +0.5f, -0.5f}, {-0.5f, +0.5f, +0.5f}, {+0.5f, +0.5f, +0.5f}, {+0.5f, +0.5f, -0.5f}}, // Top
    {{-0.5f, -0.5f, -0.5f}, {+0.5f, -0.5f, +0.5f}, {-0.5f, -0.5f, +0.5f}, {+0.5f, -0.5f, -0.5f}}, // Bottom
    {{+0.5f, -0.5f, -0.5f}, {+0.5f, +0.5f, -0.5f}, {+0.5f, +0.5f, +0.5f}, {+0.5f, -0.5f, +0.5f}}, // Left
    {{-0.5f, -0.5f, -0.5f}, {-0.5f, +0.5f, +0.5f}, {-0.5f, +0.5f, -0.5f}, {-0.5f, -0.5f, +0.5f}}, // Right
    {{-0.5f, -0.5f, +0.5f}, {+0.5f, -0.5f, +0.5f}, {+0.5f, +0.5f, +0.5f}, {-0.5f, +0.5f, +0.5f}}, // Front
    {{-0.5f, -0.5f, -0.5f}, {-0.5f, +0.5f, -0.5f}, {+0.5f, +0.5f, -0.5f}, {+0.5f, -0.5f, -0.5f}}  // Back
};

std::vector<float> Renderer::_posBuffer;
std::vector<int8_t> Renderer::_normalBuffer;
std::vector<uint8_t> Renderer::_colorBuffer;

void Renderer::generate_mesh(VoxelStructure *structure, VoxelChunk& chunk, size_t& vertices, GLuint& vao, GLuint& vboPos, GLuint& vboNormal, GLuint& vboColor) {
    _posBuffer.reserve(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 36 * 3);
	_normalBuffer.reserve(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 36 * 3);
	_colorBuffer.reserve(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 36 * 4);

	structure->forEachVisibleVoxelInChunk(chunk, [&] (Voxel &voxel, int x, int y, int z) {
		Color& clr = voxel.getColor();

		for (int i=0; i<6; i++) {
			glm::vec3 pos(x, y, z);
			glm::vec3 neighborPos = pos + neighborOffset[i];
			if (!chunk.getVoxel(neighborPos).visible) {
				quadFace(
                    pos+offsets[i][0], pos+offsets[i][1],
                    pos+offsets[i][2], pos+offsets[i][3],
                    normals[i], clr
				);
			}
		}
	});

	vertices = _posBuffer.size() / 3;

	glBindVertexArray(vao);

	// Позиции
	glBindBuffer(GL_ARRAY_BUFFER, vboPos);
	glBufferData(GL_ARRAY_BUFFER, _posBuffer.size() * sizeof(float), _posBuffer.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// Нормали
	glBindBuffer(GL_ARRAY_BUFFER, vboNormal);
	glBufferData(GL_ARRAY_BUFFER, _normalBuffer.size() * sizeof(int8_t), _normalBuffer.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_BYTE, GL_TRUE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	// Цвет
	glBindBuffer(GL_ARRAY_BUFFER, vboColor);
	glBufferData(GL_ARRAY_BUFFER, _colorBuffer.size() * sizeof(uint8_t), _colorBuffer.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);
	glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Renderer::quadFace(
    const glm::vec3& v0,
    const glm::vec3& v1,
    const glm::vec3& v2,
    const glm::vec3& v3,
    const glm::ivec3& normal,
    const Color& clr
) {
    vertex(v0, normal, clr);
    vertex(v1, normal, clr);
    vertex(v2, normal, clr);

    vertex(v0, normal, clr);
    vertex(v2, normal, clr);
    vertex(v3, normal, clr);
}

void Renderer::vertex(
	const glm::vec3 &pos, 
	const glm::ivec3 &normal, 
	const Color& clr
) {
	_posBuffer.push_back(pos.x);
	_posBuffer.push_back(pos.y);
	_posBuffer.push_back(pos.z);

	_normalBuffer.push_back(normal.x);
	_normalBuffer.push_back(normal.y);
	_normalBuffer.push_back(normal.z);

	_colorBuffer.push_back(clr.r);
	_colorBuffer.push_back(clr.g);
	_colorBuffer.push_back(clr.b);
	_colorBuffer.push_back(clr.a);
}