#include "voxelmesh.h"

#include "voxels/voxelstructure.h"

#include "graphics/renderer.h"
#include "renderer.h"

#include <iostream>

static constexpr glm::vec3 neighborOffset[6] = { {0,1,0}, {0,-1,0}, {1,0,0}, {-1,0,0}, {0,0,1}, {0,0,-1} };

static constexpr glm::ivec3 normals[6] = {
    { 0,  1,  0}, // Top (y = +0.5)
    { 0, -1,  0}, // Bottom (y = -0.5)
    { 1,  0,  0}, // Right  (x = +0.5)
    {-1,  0,  0}, // Left   (x = -0.5)
    { 0,  0,  1}, // Front  (z = +0.5)
    { 0,  0, -1}  // Back   (z = -0.5)
};

// Куб (вершины + индексы)
static const GLfloat cubeVertices[] = {
    // позиции
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f
};
static const GLuint cubeIndices[] = {
    0, 1, 2, 2, 3, 0, // back
    4, 5, 6, 6, 7, 4, // front
    0, 4, 7, 7, 3, 0, // left
    1, 5, 6, 6, 2, 1, // right
    3, 2, 6, 6, 7, 3, // top
    0, 1, 5, 5, 4, 0  // bottom
};

VoxelMesh::VoxelMesh(size_t vertices, GLuint vao, GLuint vboPos, GLuint vboNormal, GLuint vboColor):
_vertices(vertices), _vao(vao), _vboPos(vboPos), _vboNormal(vboNormal), _vboColor(vboColor) {}

VoxelMesh::VoxelMesh(VoxelChunk& chunk) {
    glGenVertexArrays(1, &__vao);
    glBindVertexArray(__vao);

    // Куб
    glGenBuffers(1, &_vboCube);
    glBindBuffer(GL_ARRAY_BUFFER, _vboCube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Индексы
    glGenBuffers(1, &_eboCube);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _eboCube);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    // Буфер инстансов (позиции + цвет)
    glGenBuffers(1, &_vboInstances);
    glBindBuffer(GL_ARRAY_BUFFER, _vboInstances);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glBindVertexArray(__vao);

    // layout = 1 → позиция
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VoxelInstance), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);

	// glBufferData(GL_ARRAY_BUFFER, _normalBuffer.size() * sizeof(VoxelInstance), _normalBuffer.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_BYTE, GL_TRUE, sizeof(VoxelInstance), (void*)offsetof(VoxelInstance, normal));
	glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    // layout = 2 → цвет
    glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VoxelInstance), (void*)offsetof(VoxelInstance, color));
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);

    glBindVertexArray(0);

    this->generateInstances(chunk);
}

void VoxelMesh::generateInstances(VoxelChunk& chunk) {
    _instances.clear();

    chunk.eachVisible([&](Voxel& voxel, glm::vec3 pos) {
        if (!voxel.visible) return;

        // проверяем: если хотя бы одна грань открыта
        for (int i=0; i<6; i++) {
			glm::vec3 neighborPos = pos + neighborOffset[i];
            if (!chunk.at(neighborPos.x, neighborPos.y, neighborPos.z).visible) {
                VoxelInstance inst;
                inst.pos = pos;
                inst.normal = normals[i];
                inst.color = glm::u8vec4(voxel.getColor().r,
                                         voxel.getColor().g,
                                         voxel.getColor().b,
                                         voxel.getColor().a);
                _instances.push_back(inst);
                break;
            }
        }
    });

    std::cout << "Instances: " << _instances.size() << "\n";

    // загружаем в GPU
    glBindBuffer(GL_ARRAY_BUFFER, _vboInstances);
    glBufferData(GL_ARRAY_BUFFER, _instances.size() * sizeof(VoxelInstance), _instances.data(), GL_DYNAMIC_DRAW);
}
void VoxelMesh::drawInstances(unsigned int primitive, glm::mat4 _modelmatrix, Shader* shader) {
    glBindVertexArray(__vao);
	if (shader != nullptr) {
        shader->uniformMatrix("model", _modelmatrix);
    }
    glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, _instances.size());
    glBindVertexArray(0);
}

VoxelMesh::~VoxelMesh() {
    if (_vao) glDeleteVertexArrays(1, &_vao);
    if (_vboPos) {
        glDeleteBuffers(1, &_vboPos);
        glDeleteBuffers(1, &_vboNormal);
        glDeleteBuffers(1, &_vboColor);
    }
    _vao = 0;
    _vboPos = 0;
    _vboNormal = 0;
    _vboColor = 0;
}

void VoxelMesh::draw(unsigned int primitive, glm::mat4 _modelmatrix, Shader* shader) {
    glBindVertexArray(_vao);
    if (shader != nullptr) {
        shader->uniformMatrix("model", _modelmatrix);
    }
    glDrawArrays(primitive, 0, this->_vertices);
    glBindVertexArray(0);
}