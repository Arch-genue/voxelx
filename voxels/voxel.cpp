#include "voxel.h"

#include <iostream>
#include <fstream>
#include <string.h>

void split(std::string* bufstr, std::string str, char separator) {
    uint16_t len = strlen(str.data());
    uint8_t d = 0;

    std::string s;
    for (uint8_t i = 0; i < len; i++) {
        if (str.data()[i] != separator) s += str.data()[i];
        else {
            bufstr[d] = s;
            s.clear();
            d++;
        }
    }
    bufstr[d] = s;
}

//TODO !!!!!!!!!!!
#define CHUNK_MODEL 10
size_t MODELSIZE = CHUNK_MODEL*CHUNK_MODEL*CHUNK_MODEL;

VoxModel::VoxModel(std::string filename) {
    load_vox(filename);
    
	for (int y = 0; y < CHUNK_MODEL; y++) {
		for (int z = 0; z < CHUNK_MODEL; z++) {
			for (int x = 0; x < CHUNK_MODEL; x++) {
                for (int i = 0; i < voxcount; i++) {
                    if (x == voxels[i].position.x + 3 && 
                        y == voxels[i].position.y + 3 &&
                        z == voxels[i].position.z + 3
                       ) voxels[(y * CHUNK_MODEL + z) * CHUNK_MODEL + x].id = 3;
                }
			}
		}
	}
}

void VoxModel::load_vox(std::string filename) {
    std::ifstream in(filename); // окрываем файл для чтения
    if (in.is_open()) {
        std::string line;
        int i = 0;
        uint16_t vi = 0;
        voxels = new Voxel[MODELSIZE];
	    nullvox();
        
        while (getline(in, line)) {
            vi++;
            if ( vi <= 3 ) continue;
            
            std::string str[4];
            split(str, line, ' '); // x y z clr

            voxels[i].position = glm::vec3( 
                atoi(str[0].c_str()),
                atoi(str[1].c_str()),
                atoi(str[2].c_str())
            );
            
            //TODO VOXEL COLOR!

            //? Test
            // std::cout << i 
            //  << " X: " << voxels[i].position.x
            //  << " Y: " << voxels[i].position.y
            //  << " Z: " << voxels[i].position.z
            //  << std::endl;
            i++;
        }
        voxcount = i;
        in.close();
    } else {
        std::cerr << "ERROR::VOXMODEL::Voxel model not loaded: " << filename << std::endl;
        //return nullptr;
    }
}

void VoxModel::nullvox() {
    for (int i = 0; i < MODELSIZE; i++) {
        voxels[i].position = glm::vec3(0);
        voxels[i].id = 0;
    }
}