#include "vox_renderer.h"
#include "mesh.h"
//#include "../voxels/chunk.h"
#include "../voxels/voxel.h"
#include <iostream>
#include <glm/glm.hpp>
//#include <glm/ext.hpp>

#define VERTEX_SIZE (3 + 4 + 1)
int chunk_attrs[] = {3,4,1, 0};

#define IS_IN(X,Y,Z) ((X) >= 0 && (Y) >= 0 && (Z) >= 0)
#define IS_BLOCKED(VOXELS,X,Y,Z) (find((VOXELS), (X), (Y), (Z)))
#define VERTEX(INDEX, X,Y,Z, C1,C2,C3,C4,L) \
					buffer[INDEX+0] = (X); \
					buffer[INDEX+1] = (Y); \
					buffer[INDEX+2] = (Z); \
					buffer[INDEX+3] = (C1);\
					buffer[INDEX+4] = (C2);\
					buffer[INDEX+5] = (C3);\
					buffer[INDEX+6] = (C4);\
					buffer[INDEX+7] = (L);\
					INDEX += VERTEX_SIZE;

ModelRenderer::ModelRenderer(size_t capacity) : capacity(capacity) {
	buffer = new float[capacity * VERTEX_SIZE * 6];
}

ModelRenderer::~ModelRenderer() {
	delete[] buffer;
}

//! OPTIMIZATION
bool find(_voxels* voxels, float x, float y, float z) {
	//return false;
	for (size_t i = 0; i < voxels->count; i++)
		if (voxels->voxels[i].position.x == x && voxels->voxels[i].position.y == y && voxels->voxels[i].position.z == z )
			return true;
	return false;
}

Mesh* ModelRenderer::render(_voxels* voxels) {
	size_t index = 0;
	float x,y,z;

	float c1 = 0.0f;
	float c2 = 0.0f;
	float c3 = 0.0f;
	float c4 = 1.0f;
	float l = 1.0f;
	//float l = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	for (size_t i = 0; i < voxels->count; i++) {
		x = voxels->voxels[i].position.x;
		y = voxels->voxels[i].position.y;
		z = voxels->voxels[i].position.z;

		c1 = voxels->voxels[i].clr.x;
		c2 = voxels->voxels[i].clr.y;
		c3 = voxels->voxels[i].clr.z;
		c4 = voxels->voxels[i].clr.w;

		//* 0.5f 0.5f 0.0f 1.0f -- vomit
		//* 0.3f 0.3f 1.0f 0.5f -- water
		//* 0.3f 0.7f 1.0f 0.5f -- water2
		
		//? Y
		if (!IS_BLOCKED(voxels, x,y+1,z)) {
			l = 1.0f;
			VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4,l);

			VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, c1,c2,c3,c4,l);
		} 
		if (!IS_BLOCKED(voxels, x,y-1,z)) {
			l = 0.75f;
			VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, c1,c2,c3,c4,l);

			VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, c1,c2,c3,c4,l);
		}

		//? X
		if (!IS_BLOCKED(voxels, x+1,y,z)) {
			l = 0.95f;
			VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4,l);

			VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, c1,c2,c3,c4,l);
		}
		if (!IS_BLOCKED(voxels, x-1,y,z)) {
			l = 0.85f;
			VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, c1,c2,c3,c4,l);

			VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4,l);
		}

		//? Z
		if (!IS_BLOCKED(voxels, x,y,z+1)) {
			l = 0.9f;
			VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4,l);

			VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4,l);
		}
		if (!IS_BLOCKED(voxels, x,y,z-1)) {
			l = 0.8f;
			VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, c1,c2,c3,c4,l);

			VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4,l);
		}
	}
	return new Mesh(buffer, index / VERTEX_SIZE, chunk_attrs);
}