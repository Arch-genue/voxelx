#include "vox_renderer.h"
#include "mesh.h"
//#include "../voxels/chunk.h"
#include "../voxels/voxel.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define VERTEX_SIZE (3 + 4 + 1)

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
								//buffer[INDEX+6] = (C4);\
								

ModelRenderer::ModelRenderer(size_t capacity) : capacity(capacity) {
	buffer = new float[capacity * VERTEX_SIZE * 6];
}

ModelRenderer::~ModelRenderer() {
	delete[] buffer;
}

int chunk_attrs1[] = {3,4,1, 0};

//#define CHUNK_MODEL 10

Mesh* ModelRenderer::render(VoxModel* model) {
	size_t index = 0;
	float c1 = 0.0f;
	float c2 = 0.0f;
	float c3 = 0.0f;
	float c4 = 1.0f;
	float l = 0.3f;

	float x,y,z;
	for (int i = 0; i < model->voxcount; i++) {
		// if (
		// 	x == model->voxels[i].position.x &&
		// 	y == model->voxels[i].position.y &&
		// 	z == model->voxels[i].position.z
		// 	) {
			//Voxel vox = model->voxels[i];
			c1 = model->voxels[i].clr.x;
			c2 = model->voxels[i].clr.y;
			c3 = model->voxels[i].clr.z;
			c4 = model->voxels[i].clr.w;
			
			
			x = model->voxels[i].position.x;
			y = model->voxels[i].position.y;
			z = model->voxels[i].position.z;
			
			// std::cout << 
			// x << " " <<
			// y << " " <<
			// z << " " <<
			// vox.clr.x << " " << 
			// vox.clr.y << " " << 
			// vox.clr.z << " " <<
			// vox.clr.w <<
			// std::endl;

			//* 0.5f 0.5f 0.0f 1.0f -- vomit
			//* 0.3f 0.3f 1.0f 0.5f -- water
			//* 0.3f 0.7f 1.0f 0.5f -- water2
			
			//* Colors
			// c1 = 0.1f*x;
			// c2 = 0.2f*y;
			// c3 = 0.2f*z;
			// c4 = 0.5f;
			//continue;
			VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4,l);

			VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			//
			VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, c1,c2,c3,c4,l);

			VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, c1,c2,c3,c4,l);
			//
			VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4,l);

			VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, c1,c2,c3,c4,l);
			//
			VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, c1,c2,c3,c4,l);

			VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4,l);
			//
			VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4,l);

			VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, c1,c2,c3,c4,l);
			//
			VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, c1,c2,c3,c4,l);

			VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, c1,c2,c3,c4,l);
			VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, c1,c2,c3,c4,l);
		//}
	}
	// 		}
	// 	}
	// }
	return new Mesh(buffer, index / VERTEX_SIZE, chunk_attrs1);
}