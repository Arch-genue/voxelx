#include "vox_renderer.h"
#include "mesh.h"
//#include "../voxels/chunk.h"
#include "../voxels/voxel.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define VERTEX_SIZE (3 + 3)

#define VERTEX(INDEX, X,Y,Z, C1,C2,C3,C4) \
								buffer[INDEX+0] = (X);\
								buffer[INDEX+1] = (Y);\
								buffer[INDEX+2] = (Z);\
								buffer[INDEX+3] = (C1);\
								buffer[INDEX+4] = (C2);\
								buffer[INDEX+5] = (C3);\
								buffer[INDEX+6] = (C4);\
								INDEX += VERTEX_SIZE;

ModelRenderer::ModelRenderer(size_t capacity) : capacity(capacity) {
	buffer = new float[capacity * VERTEX_SIZE * 6];
}

ModelRenderer::~ModelRenderer() {
	delete[] buffer;
}

int chunk_attrs1[] = {3,2,1, 0};

#define CHUNK_MODEL 10

Mesh* ModelRenderer::render(VoxModel* chunk) {
	size_t index = 0;
	unsigned int id = 0;
	float c1 = 0.0f;
	float c2 = 0.0f;
	float c3 = 0.0f;
	float c4 = 1.0f;
	for (int y = 0; y < CHUNK_MODEL; y++) {
		for (int z = 0; z < CHUNK_MODEL; z++) {
			for (int x = 0; x < CHUNK_MODEL; x++) {
				Voxel vox = chunk->voxels[(y * CHUNK_MODEL + z) * CHUNK_MODEL + x];
				//id = chunk->voxels[(y * CHUNK_MODEL + z) * CHUNK_MODEL + x].id;
				id = vox.id;

				if (!id) continue;
				
				// float c1 = 0.1f*x;
				// float c2 = 0.1f*z;
				// float c3 = 0.2f*y;
				float c1 = 0.0f;
				float c2 = 0.0f;
				float c3 = 0.0f;
				float c4 = 0.5f;

				VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, c1,c2,c3,c4);
				VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4);
				VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4);

				VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, c1,c2,c3,c4);
				VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4);
				VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, c1,c2,c3,c4);
				//
				VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4);
				VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, c1,c2,c3,c4);
				VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, c1,c2,c3,c4);

				VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4);
				VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4);
				VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, c1,c2,c3,c4);
				//
				VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4);
				VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, c1,c2,c3,c4);
				VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4);

				VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4);
				VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4);
				VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, c1,c2,c3,c4);
				//
				VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4);
				VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4);
				VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, c1,c2,c3,c4);

				VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4);
				VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, c1,c2,c3,c4);
				VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4);
				//
				VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, c1,c2,c3,c4);
				VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4);
				VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4);

				VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, c1,c2,c3,c4);
				VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, c1,c2,c3,c4);
				VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4);
				//
				VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4);
				VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, c1,c2,c3,c4);
				VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, c1,c2,c3,c4);

				VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4);
				VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, c1,c2,c3,c4);
				VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4);
			}
		}
	}
	return new Mesh(buffer, index / VERTEX_SIZE, chunk_attrs1);
}