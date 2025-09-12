#include "renderer.h"

#include <iostream>

// static constexpr glm::ivec3 normals[6] = { {0,1,0}, {0,-1,0}, {1,0,0}, {-1,0,0}, {0,0,1}, {0,0,-1} };
static constexpr glm::ivec3 normals[6] = {
    { 0,  1,  0}, // Top (y = +0.5)
    { 0, -1,  0}, // Bottom (y = -0.5)
    { 1,  0,  0}, // Right  (x = +0.5)
    {-1,  0,  0}, // Left   (x = -0.5)
    { 0,  0,  1}, // Front  (z = +0.5)
    { 0,  0, -1}  // Back   (z = -0.5)
};

static constexpr glm::vec3 neighborOffset[6] = { {0,1,0}, {0,-1,0}, {1,0,0}, {-1,0,0}, {0,0,1}, {0,0,-1} };

static constexpr glm::vec3 offsets[6][4] = {
    // Top (y = +0.5)
    {{-0.5f, +0.5f, -0.5f}, {+0.5f, +0.5f, -0.5f}, {+0.5f, +0.5f, +0.5f}, {-0.5f, +0.5f, +0.5f}},
    // Bottom (y = -0.5)
    {{-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, +0.5f}, {+0.5f, -0.5f, +0.5f}, {+0.5f, -0.5f, -0.5f}},
    // Right (x = +0.5)
    {{+0.5f, -0.5f, -0.5f}, {+0.5f, -0.5f, +0.5f}, {+0.5f, +0.5f, +0.5f}, {+0.5f, +0.5f, -0.5f}},
    // Left (x = -0.5)
    {{-0.5f, -0.5f, -0.5f}, {-0.5f, +0.5f, -0.5f}, {-0.5f, +0.5f, +0.5f}, {-0.5f, -0.5f, +0.5f}},
    // Front (z = +0.5)
    {{-0.5f, -0.5f, +0.5f}, {-0.5f, +0.5f, +0.5f}, {+0.5f, +0.5f, +0.5f}, {+0.5f, -0.5f, +0.5f}},
    // Back (z = -0.5)
    {{-0.5f, -0.5f, -0.5f}, {+0.5f, -0.5f, -0.5f}, {+0.5f, +0.5f, -0.5f}, {-0.5f, +0.5f, -0.5f}}
};

std::vector<float> Renderer::_posBuffer;
std::vector<int8_t> Renderer::_normalBuffer;
std::vector<uint8_t> Renderer::_colorBuffer;

void Renderer::reserve() {
	_posBuffer.reserve(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 36 * 3);
	_normalBuffer.reserve(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 36 * 3);
	_colorBuffer.reserve(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 36 * 4);
}

std::unique_ptr<VoxelMesh> Renderer::generate_mesh(VoxelChunk& chunk) {
	chunk.eachVisible([&](Voxel& voxel, glm::vec3 pos) {
		Color& clr = voxel.getColor();

		for (int i=0; i<6; i++) {
			glm::vec3 neighborPos = pos + neighborOffset[i];
			if (!chunk.at(neighborPos).visible) {
				quadFace(
                    pos+offsets[i][0], pos+offsets[i][1],
                    pos+offsets[i][2], pos+offsets[i][3],
                    normals[i], clr
				);
			}
		}
	});

	size_t vertices = _posBuffer.size() / 3;
	GLuint vao;
	GLuint vboPos;
	GLuint vboNormal;
	GLuint vboColor;

	glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vboPos);
    glGenBuffers(1, &vboNormal);
    glGenBuffers(1, &vboColor);

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

	_posBuffer.clear();
	_normalBuffer.clear();
	_colorBuffer.clear();

	return std::make_unique<VoxelMesh>(vertices, vao, vboPos, vboNormal, vboColor);
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