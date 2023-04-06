#include "chunk.h"
#include "voxel.h"
#include <math.h>
#include <iostream>

Chunk::Chunk(int xpos, int ypos, int zpos) : x(xpos), y(ypos), z(zpos) {
	voxels = new Voxel[CHUNK_VOL];
	//null(voxels);

	for (int y = 0; y < CHUNK_H; y++){
		for (int z = 0; z < CHUNK_D; z++){
			for (int x = 0; x < CHUNK_W; x++){
				int real_x = x + this->x * CHUNK_W;
				int real_y = y + this->y * CHUNK_H;
				int real_z = z + this->z * CHUNK_D;
				int id = real_y <= (sin(real_x*0.1f) * 0.5f + 0.5f) * 10;
				if (real_y <= 2)
					id = 2;

				//std::cout << "DDDD: " << (y * CHUNK_D + z) * CHUNK_W + x << std::endl;
				voxels[(y * CHUNK_D + z) * CHUNK_W + x].id = id;
			}
		}
	}
}

void Chunk::null(Voxel* voxel) {
	for (int y = 0; y < CHUNK_H; y++) {
		for (int z = 0; z < CHUNK_D; z++) {
			for (int x = 0; x < CHUNK_W; x++) {
				voxels[(y * CHUNK_D + z) * CHUNK_W + x].id = 0;
			}
		}
	}
}

Chunk::~Chunk() {
	delete[] voxels;
}