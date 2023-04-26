#include "renderer.h"
#include "mesh.h"
#include "../voxels/voxel.h"
#include <glm/glm.hpp>

#include <iostream>

#define VERTEX_SIZE (3 + 4 + 3)
int chunk_attrs[] = {3,4,3, 0};

#define IS_IN(X,Y,Z) ((X) >= 0 && (X) < voxels->m_size.x && (Y) >= 0 && (Y) < voxels->m_size.y && (Z) >= 0 && (Z) < voxels->m_size.z)
#define VOXEL(X,Y,Z) (voxels->voxels[((Y) * (int)voxels->m_size.z + (Z)) * (int)voxels->m_size.x + (X)])
#define IS_BLOCKED(X,Y,Z) (false) //((IS_IN(X, Y, Z)) && VOXEL(X, Y, Z).visible) //(false)//

#define VERTEX(INDEX, X,Y,Z, CLR, LIGHT) \
					buffer[INDEX+0] = (X); \
					buffer[INDEX+1] = (Y); \
					buffer[INDEX+2] = (Z); \
					buffer[INDEX+3] = (CLR).x;\
					buffer[INDEX+4] = (CLR).y;\
					buffer[INDEX+5] = (CLR).z;\
					buffer[INDEX+6] = (CLR).w;\
					buffer[INDEX+7] = (LIGHT).x;\
					buffer[INDEX+8] = (LIGHT).y;\
					buffer[INDEX+9] = (LIGHT).z;\
					INDEX += VERTEX_SIZE;

float* Renderer::buffer;
size_t Renderer::capacity;

Camera* Renderer::camera;

void Renderer::init(size_t capacity) {
	buffer = new float[capacity * VERTEX_SIZE * 6];
	Renderer::capacity = capacity;
}
void Renderer::free() {
	delete[] buffer;

}
void Renderer::addCamera(Camera* cam) {
	camera = cam;
}

Mesh* Renderer::render(_voxels* voxels) {
	size_t index {0};
	float x, y, z;
	glm::vec4 clr;
	glm::vec3 light;
	bool side[6]; //! top, bottom, left, right, front, back
	
	for(uint8_t i = 0; i < 6; i++) side[i] = false;

	for (size_t i = 0; i < voxels->voxels.size(); i++) {
		if (!voxels->voxels[i].visible) continue;

		x = voxels->voxels[i].position.x;
		y = voxels->voxels[i].position.y;
		z = voxels->voxels[i].position.z;
		clr = voxels->voxels[i].clr;

		//glm::vec3(0.0f, 0.0f, 0.0f);

		if (voxels->renderSide == "") {
			for(uint8_t i = 0; i < 6; i++) side[i] = true;
		} else if(voxels->renderSide == "top") {
			side[0] = true;
		} else if(voxels->renderSide == "bottom") {
			side[1] = true;
		} else if(voxels->renderSide == "left") {
			side[2] = true;
		} else if(voxels->renderSide == "right") {
			side[3] = true;
		} else if(voxels->renderSide == "front") {
			side[4] = true;
		} else if(voxels->renderSide == "back") {
			side[5] = true;
		}

		//std::cout << voxels->renderSide << std::endl;
		
		//? Y
		if (!IS_BLOCKED(x,y+1,z) && side[0] == true) {
			light = voxels->light[0];
			top(index, x, y, z, clr, light);
		} 
		if (!IS_BLOCKED(x,y-1,z) && side[1] == true) {
			light = voxels->light[1];
			bottom(index, x, y, z, clr, light);
		}

		//? X
		if (!IS_BLOCKED(x+1,y,z) && side[2] == true) {
			light = voxels->light[2];
			left(index, x, y, z, clr, light);
		}
		if (!IS_BLOCKED(x-1,y,z) && side[3] == true) {
			light = voxels->light[3];
			right(index, x, y, z, clr, light);
		}

		//? Z
		if (!IS_BLOCKED(x,y,z+1) && side[4] == true) {
			light = voxels->light[4];
			front(index, x, y, z, clr, light);
		}
		if (!IS_BLOCKED(x,y,z-1) && side[5] == true) {
			light = voxels->light[5];
			back(index, x, y, z, clr, light);
		}
	}
	return new Mesh(voxels, buffer, index / VERTEX_SIZE, chunk_attrs);
}

void Renderer::top(size_t &index, float x, float y, float z, glm::vec4 clr, glm::vec3 light) {
	VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, clr, light);
	VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, clr, light);
	VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, clr, light);

	VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, clr, light);
	VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, clr, light);
	VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, clr, light);
}
void Renderer::bottom(size_t &index, float x, float y, float z, glm::vec4 clr, glm::vec3 light) {
	VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, clr, light);
	VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, clr, light);
	VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, clr, light);

	VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, clr, light);
	VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, clr, light);
	VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, clr, light);
}
void Renderer::left(size_t &index, float x, float y, float z, glm::vec4 clr, glm::vec3 light) {
	VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, clr, light);
	VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, clr, light);
	VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, clr, light);

	VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, clr, light);
	VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, clr, light);
	VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, clr, light);
}
void Renderer::right(size_t &index, float x, float y, float z, glm::vec4 clr, glm::vec3 light) {
	VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, clr, light);
	VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, clr, light);
	VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, clr, light);

	VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, clr, light);
	VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, clr, light);
	VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, clr, light);
}
void Renderer::front(size_t &index, float x, float y, float z, glm::vec4 clr, glm::vec3 light) {
	VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, clr, light);
	VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, clr, light);
	VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, clr, light);

	VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, clr, light);
	VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, clr, light);
	VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, clr, light);
}
void Renderer::back(size_t &index, float x, float y, float z, glm::vec4 clr, glm::vec3 light) {
	VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, clr, light);
	VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, clr, light);
	VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, clr, light);

	VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, clr, light);
	VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, clr, light);
	VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, clr, light);
}

Camera* Renderer::getCamera() {
	return camera;
}