#include "vox_renderer.h"
#include "mesh.h"
//#include "../voxels/chunk.h"
#include "../voxels/voxel.h"
#include <iostream>
#include <glm/glm.hpp>
//#include <glm/ext.hpp>

#define VERTEX_SIZE (3 + 4 + 1)
int chunk_attrs[] = {3,4,1, 0};

#define IS_IN(X,Y,Z) ((X) >= 0 && (X) < voxels->m_size.x && (Y) >= 0 && (Y) < voxels->m_size.y && (Z) >= 0 && (Z) < voxels->m_size.z)
#define VOXEL(X,Y,Z) (voxels->voxels[((Y) * (int)voxels->m_size.z + (Z)) * (int)voxels->m_size.x + (X)])
#define IS_BLOCKED(X,Y,Z) ((IS_IN(X, Y, Z)) && VOXEL(X, Y, Z).visible)

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

ModelRenderer::ModelRenderer(size_t capacity) : capacity(capacity) {
	buffer = new float[capacity * VERTEX_SIZE * 6];
}

ModelRenderer::~ModelRenderer() {
	delete[] buffer;
}

Mesh* ModelRenderer::render(_voxels* voxels) {
	size_t index = 0;
	size_t id;

	glm::vec4 clr;
	float l = 1.0f;

	//float l = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	for (int y = 0; y < voxels->m_size.y; y++) {
    	for (int z = 0; z < voxels->m_size.z; z++) {
			for (int x = 0; x < voxels->m_size.x; x++) {
				id = (y * (int)voxels->m_size.z + z) * (int)voxels->m_size.x + x;
				if (!voxels->voxels[id].visible) continue;

				clr = voxels->voxels[id].clr;

				//* 0.5f 0.5f 0.0f 1.0f -- vomit
				//* 0.3f 0.3f 1.0f 0.5f -- water
				//* 0.3f 0.7f 1.0f 0.5f -- water2
				
				//? Y
				if (!IS_BLOCKED(x,y+1,z)) {
					l = 1.0f;
					VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, clr,l);
					VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, clr,l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, clr,l);

					VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, clr,l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, clr,l);
					VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, clr,l);
				} 
				if (!IS_BLOCKED(x,y-1,z)) {
					l = 0.75f;
					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, clr,l);
					VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, clr,l);
					VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, clr,l);

					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, clr,l);
					VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, clr,l);
					VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, clr,l);
				}

				//? X
				if (!IS_BLOCKED(x+1,y,z)) {
					l = 0.95f;
					VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, clr,l);
					VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, clr,l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, clr,l);

					VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, clr,l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, clr,l);
					VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, clr,l);
				}
				if (!IS_BLOCKED(x-1,y,z)) {
					l = 0.85f;
					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, clr,l);
					VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, clr,l);
					VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, clr,l);

					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, clr,l);
					VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, clr,l);
					VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, clr,l);
				}

				//? Z
				if (!IS_BLOCKED(x,y,z+1)) {
					l = 0.9f;
					VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, clr,l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, clr,l);
					VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, clr,l);

					VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, clr,l);
					VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, clr,l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, clr,l);
				}
				if (!IS_BLOCKED(x,y,z-1)) {
					l = 0.8f;
					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, clr,l);
					VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, clr,l);
					VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, clr,l);

					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, clr,l);
					VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, clr,l);
					VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, clr,l);
				}
			}
		}
	}
	return new Mesh(voxels, buffer, index / VERTEX_SIZE, chunk_attrs);
}