#include "vox_renderer.h"
#include "mesh.h"
//#include "../voxels/chunk.h"
#include "../voxels/voxel.h"
#include <iostream>
#include <glm/glm.hpp>

#define VERTEX_SIZE (3 + 4 + 1)
int chunk_attrs[] = {3,4,1, 0};

#define IS_IN(X,Y,Z) ((X) >= 0 && (X) < voxels->m_size.x && (Y) >= 0 && (Y) < voxels->m_size.y && (Z) >= 0 && (Z) < voxels->m_size.z)
#define VOXEL(X,Y,Z) (voxels->voxels[((Y) * (int)voxels->m_size.z + (Z)) * (int)voxels->m_size.x + (X)])
#define IS_BLOCKED(X,Y,Z) (false)//((IS_IN(X, Y, Z)) && VOXEL(X, Y, Z).visible)

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

VoxelRenderer::VoxelRenderer(size_t capacity) : capacity(capacity) {
	buffer = new float[capacity * VERTEX_SIZE * 6];
}

VoxelRenderer::~VoxelRenderer() {
	delete[] buffer;
}

Mesh* VoxelRenderer::voxelRender(voxel_m* voxel) {
	size_t index = 0;
	float x, y, z;

	glm::vec4 clr;
	float l = 1.0f;

	x = voxel->position.x;
	y = voxel->position.y;
	z = voxel->position.z;
	clr = voxel->clr;
	//_voxels* voxels = new _voxels;
	//voxels.voxels.push_back(voxel);

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
	return new Mesh(buffer, index / VERTEX_SIZE, chunk_attrs);
}

Mesh* VoxelRenderer::render(_voxels* voxels) {
	size_t index = 0;
	float x, y, z;

	glm::vec4 clr;
	float l = 1.0f;

	for (size_t i = 0; i < voxels->voxels.size(); i++) {
		//if (!voxels->voxels[i].visible) continue;

		x = voxels->voxels[i].position.x;
		y = voxels->voxels[i].position.y;
		z = voxels->voxels[i].position.z;
		clr = voxels->voxels[i].clr;

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
	return new Mesh(buffer, index / VERTEX_SIZE, chunk_attrs);
}