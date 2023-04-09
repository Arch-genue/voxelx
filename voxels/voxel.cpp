#include "voxel.h"

#include <iostream>
#include <fstream>
#include <string.h>
#include <cstring>
#include <cmath>
#include <array>

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
Model::Model() {

}
Model::~Model() {
    //delete voxels;
}

_voxels* gen_wall() {
    short x_min = 999; short y_min = 999; short z_min = 999;
    short x_coord, y_coord, z_coord;
    std::string str[5];
    size_t vi = 0;

    _voxels* voxels = new _voxels;
    voxels->count = 15000;
    voxels->voxels = new voxel_m[voxels->count];
    
    float clr_r, clr_g, clr_b, clr_a;

    clr_a = 1.0f;
    for(uint16_t x2 = 0; x2 < 50; x2++) {
        for(uint16_t y2 = 0; y2 < 100; y2++) {
            for(uint16_t z2 = 0; z2 < 3; z2++) {
                clr_r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                clr_g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                clr_b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

                x_coord = x2;
                y_coord = y2;
                z_coord = z2;

                if (x_coord < 0 && x_coord < x_min) x_min = x_coord;
                if (y_coord < 0 && y_coord < y_min) y_min = y_coord;
                if (z_coord < 0 && z_coord < z_min) z_min = z_coord;

                voxels->voxels[vi].position = glm::vec3(x_coord, y_coord, z_coord);
                voxels->voxels[vi].clr = glm::vec4(clr_r, clr_g, clr_b, clr_a);

                vi++;
            }
        }
    }
    if ( x_min == 999 ) x_min = 0;
    if ( y_min == 999 ) y_min = 0;
    if ( z_min == 999 ) z_min = 0;
    for (size_t i = 0; i < voxels->count; i++) {
        voxels->voxels[i].position.x += abs(x_min);
        voxels->voxels[i].position.y += abs(y_min);
        voxels->voxels[i].position.z += abs(z_min);
        std::cout << x_min << std::endl;
    }
    return voxels;
}

_voxels* load_model(std::string filename, const char* type, bool test) {
    if (test) return gen_wall();

    std::ifstream in(filename); // окрываем файл для чтения
    if (in.is_open()) {
        std::string line;
        uint16_t vi = 0;

        short x_coord, y_coord, z_coord;
        short x_min = 999; short y_min = 999; short z_min = 999;
        //short x_max = -999; short y_max = -999; short z_max = -999;
        float clr_r = 0.0f, clr_g = 0.0f, clr_b = 0.0f, clr_a = 0.0f;
        
        _voxels* voxels;
        voxels = new _voxels;

        std::string str[5];
        
        vi = 0;
        while (getline(in, line)) {
            vi++;
            if ( vi == 4 ) voxels->count = atoi(line.c_str()); 
            if ( vi < 5 ) continue;
            if ( vi == 5 ) voxels->voxels = new voxel_m[voxels->count];

            split(str, line, ' '); // x y z clr

            x_coord = atoi(str[0].c_str());
            y_coord = atoi(str[1].c_str());
            z_coord = atoi(str[2].c_str());

            if (x_coord < 0 && x_coord < x_min) x_min = x_coord;
            if (y_coord < 0 && y_coord < y_min) y_min = y_coord;
            if (z_coord < 0 && z_coord < z_min) z_min = z_coord;

            voxels->voxels[vi-5].position = glm::vec3(x_coord, y_coord, z_coord);
            
            // VOXEL COLOR
            const char* clr = str[3].c_str();
            char r[2] = ""; char g[2] = "";
            strncat(r, clr, 2);
            clr_r = round( strtoul(r, NULL, 16) / 255.0 * 100 ) / 100;
            strncat(g, clr+2, 2);
            clr_g = round( strtoul(g, NULL, 16) / 255.0 * 100 ) / 100;
            clr_b = round( strtoul(clr+4, NULL, 16) / 255.0 * 100 ) / 100;
            clr_a = 1.0f;

            voxels->voxels[vi-5].clr = glm::vec4(clr_r, clr_g, clr_b, clr_a);
        }
        if ( x_min == 999 ) x_min = 0;
        if ( y_min == 999 ) y_min = 0;
        if ( z_min == 999 ) z_min = 0;
        // if ( x_max == -999 ) x_max = 0;
        // if ( y_max == -999 ) y_max = 0;
        // if ( z_max == -999 ) z_max = 0;

        for (size_t i = 0; i < voxels->count; i++) {
            voxels->voxels[i].position.x += abs(x_min);
            voxels->voxels[i].position.y += abs(y_min);
            voxels->voxels[i].position.z += abs(z_min);
            // std::cout << filename << " "  << 
            // voxels->voxels[i].position.x << " " << 
            // voxels->voxels[i].position.y << " " << 
            // voxels->voxels[i].position.z << std::endl;

            // if (voxels->voxels[i].position.x > x_max) x_max = voxels->voxels[i].position.x;
            // if (voxels->voxels[i].position.y > y_max) y_max = voxels->voxels[i].position.y;
            // if (voxels->voxels[i].position.z > z_max) z_max = voxels->voxels[i].position.z;
        }
        // m_size.x = x_max+1;
        // m_size.y = y_max+1;
        // m_size.z = z_max+1;

        in.close();
        return voxels;
    } else {
        std::cerr << "ERROR::VOXMODEL::Voxel model not loaded: " << filename << std::endl;
        return nullptr;
    }
}