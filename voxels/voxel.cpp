#include "voxel.h"

#include <iostream>
#include <fstream>
#include <string.h>
#include <cstring>
#include <cmath>

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

VoxModel::VoxModel(std::string filename, bool test) {
    if (!test) load_vox(filename);
    else gen();
}
VoxModel::~VoxModel() {
    delete voxels;
}

void VoxModel::gen() {
    float x_min, y_min, z_min = 0;
    float x_max, y_max, z_max = -999;
    int16_t x_coord, y_coord, z_coord;
    std::string str[5];
    int vi = 0;
    voxcount = 15000;
    voxels = new Voxel[voxcount];
    float clr_r, clr_g, clr_b, clr_a = 0.0f;
    clr_r = 0.3f;
    clr_g = 0.3f;
    clr_b = 0.3f;
    // clr_r = 0.3f;
    // clr_g = 0.7f;
    // clr_b = 1.0f;
    clr_a = 1.0f;
    for(int x2 = 0; x2 < 100; x2++) {
        for(int y2 = 0; y2 < 50; y2++) {
            for(int z2 = 0; z2 < 3; z2++) {
                x_coord = x2;
                y_coord = y2;
                z_coord = z2;

                if (x_coord < 0 && x_coord < x_min) x_min = x_coord;
                if (y_coord < 0 && y_coord < y_min) y_min = y_coord;
                if (z_coord < 0 && z_coord < z_min) z_min = z_coord;

                voxels[vi].position = glm::vec3( 
                    x_coord, // x
                    y_coord, // y
                    z_coord  // z
                );

                voxels[vi].clr = glm::vec4( 
                    clr_r, // r
                    clr_g, // g
                    clr_b, // b
                    clr_a  // a
                );

                vi++;
            }
        }
    }
    for (int i = 0; i < voxcount; i++) {
        voxels[i].position.x += abs(x_min);
        voxels[i].position.y += abs(y_min);
        voxels[i].position.z += abs(z_min);

        if (voxels[i].position.x > x_max) x_max = voxels[i].position.x;
        if (voxels[i].position.y > y_max) y_max = voxels[i].position.y;
        if (voxels[i].position.z > z_max) z_max = voxels[i].position.z;
    }
    m_size.x = x_max+1;
    m_size.y = y_max+1;
    m_size.z = z_max+1;
}

void VoxModel::load_vox(std::string filename) {
    std::ifstream in(filename); // окрываем файл для чтения
    if (in.is_open()) {
        std::string line;
        uint16_t vi = 0;

        float x_min, y_min, z_min = 0;
        float x_max, y_max, z_max = -999;
        int16_t x_coord, y_coord, z_coord;
        std::string str[5];
        
        vi = 0;
        while (getline(in, line)) {
            vi++;
            if ( vi == 4 ) voxcount = atoi(line.c_str()); 

            if ( vi < 5 ) continue;

            if ( vi == 5 ) {
                voxels = new Voxel[voxcount];
                nullvox(voxcount);
            }
            
            split(str, line, ' '); // x y z clr

            x_coord = atoi(str[0].c_str());
            y_coord = atoi(str[1].c_str());
            z_coord = atoi(str[2].c_str());

            if (x_coord < 0 && x_coord < x_min) x_min = x_coord;
            if (y_coord < 0 && y_coord < y_min) y_min = y_coord;
            if (z_coord < 0 && z_coord < z_min) z_min = z_coord;

            voxels[vi-5].position = glm::vec3( 
                x_coord, // x
                y_coord, // y
                z_coord  // z
            );
            
            // VOXEL COLOR
            float clr_r, clr_g, clr_b, clr_a = 0.0f;
            const char* clr = str[3].c_str();
            char r[2] = ""; char g[2] = "";
            strncat(r, clr, 2);
            clr_r = round( strtoul(r, NULL, 16) / 255.0 * 100 ) / 100;
            strncat(g, clr+2, 2);
            clr_g = round( strtoul(g, NULL, 16) / 255.0 * 100 ) / 100;
            clr_b = round( strtoul(clr+4, NULL, 16) / 255.0 * 100 ) / 100;
            clr_a = 1.0f;//str[5] != "" ? round( atoi(str[5].c_str()) / 255.0 * 100 ) / 100 : 1.0f;

            voxels[vi-5].clr = glm::vec4( 
                clr_r, // r
                clr_g, // g
                clr_b, // b
                clr_a  // a
            );
        }
        for (int i = 0; i < voxcount; i++) {
            voxels[i].position.x += abs(x_min);
            voxels[i].position.y += abs(y_min);
            voxels[i].position.z += abs(z_min);

            if (voxels[i].position.x > x_max) x_max = voxels[i].position.x;
            if (voxels[i].position.y > y_max) y_max = voxels[i].position.y;
            if (voxels[i].position.z > z_max) z_max = voxels[i].position.z;
        }
        m_size.x = x_max+1;
        m_size.y = y_max+1;
        m_size.z = z_max+1;

        in.close();
    } else {
        std::cerr << "ERROR::VOXMODEL::Voxel model not loaded: " << filename << std::endl;
        //return nullptr;
    }
}

void VoxModel::nullvox(int voxcount) {
    for (int i = 0; i < voxcount; i++) {
        voxels[i].id = 0;
        voxels[i].position = glm::vec3(0);
        voxels[i].clr = glm::vec4(0);
    }
}