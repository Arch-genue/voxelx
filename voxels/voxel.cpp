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

_voxels* load_model(std::string filename, const char* type) {
    std::ifstream in(filename); // окрываем файл для чтения
    if (in.is_open()) {
        std::string line;
        uint16_t vi = 0;

        short x_coord, y_coord, z_coord;
        short x_min = 999; short y_min = 999; short z_min = 999;
        short x_max = -999; short y_max = -999; short z_max = -999;
        float clr_r = 0.0f, clr_g = 0.0f, clr_b = 0.0f, clr_a = 0.0f;
        
        _voxels* voxels;
        voxels = new _voxels;
        voxel_m* tmpvoxs;

        std::string str[5];

        vi = 0;
        while (getline(in, line)) {
            vi++;
            if ( vi == 4 ) voxels->count = atoi(line.c_str()); 
            if ( vi < 5 ) continue;
            if ( vi == 5 ) tmpvoxs = new voxel_m[voxels->count];

            split(str, line, ' '); // x y z clr

            x_coord = atoi(str[0].c_str());
            y_coord = atoi(str[1].c_str());
            z_coord = atoi(str[2].c_str());

            if (x_coord < 0 && x_coord < x_min) x_min = x_coord;
            if (y_coord < 0 && y_coord < y_min) y_min = y_coord;
            if (z_coord < 0 && z_coord < z_min) z_min = z_coord;

            tmpvoxs[vi-5].position = glm::vec3(x_coord, y_coord, z_coord);
            
            //* VOXEL COLOR
            const char* clr = str[3].c_str();
            char r[2] = ""; char g[2] = "";
            strncat(r, clr, 2);
            clr_r = round( strtoul(r, NULL, 16) / 255.0 * 100 ) / 100;
            strncat(g, clr+2, 2);
            clr_g = round( strtoul(g, NULL, 16) / 255.0 * 100 ) / 100;
            clr_b = round( strtoul(clr+4, NULL, 16) / 255.0 * 100 ) / 100;
            clr_a = 1.0f;

            tmpvoxs[vi-5].clr = glm::vec4(clr_r, clr_g, clr_b, clr_a);
        }
        if ( x_min == 999 ) x_min = 0;
        if ( y_min == 999 ) y_min = 0;
        if ( z_min == 999 ) z_min = 0;
        if ( x_max == -999 ) x_max = 0;
        if ( y_max == -999 ) y_max = 0;
        if ( z_max == -999 ) z_max = 0;

        for (size_t i = 0; i < voxels->count; i++) {
            tmpvoxs[i].position.x += abs(x_min);
            tmpvoxs[i].position.y += abs(y_min);
            tmpvoxs[i].position.z += abs(z_min);
            // std::cout << filename << " "  << 
            // voxels->voxels[i].position.x << " " << 
            // voxels->voxels[i].position.y << " " << 
            // voxels->voxels[i].position.z << std::endl;

            if (tmpvoxs[i].position.x > x_max) x_max = tmpvoxs[i].position.x;
            if (tmpvoxs[i].position.y > y_max) y_max = tmpvoxs[i].position.y;
            if (tmpvoxs[i].position.z > z_max) z_max = tmpvoxs[i].position.z;
        }

        voxels->m_size.x = x_max+1;
        voxels->m_size.y = y_max+1;
        voxels->m_size.z = z_max+1;

        size_t count = voxels->m_size.x*voxels->m_size.y*voxels->m_size.z;

        voxels->voxels = new voxel_m[count];
        
        for (size_t i = 0; i < voxels->count; i++) {
            voxels->voxels[((int)tmpvoxs[i].position.y * (int)voxels->m_size.z + (int)tmpvoxs[i].position.z) * (int)voxels->m_size.x + (int)tmpvoxs[i].position.x].clr = glm::vec4(tmpvoxs[i].clr.x, tmpvoxs[i].clr.y, tmpvoxs[i].clr.z, tmpvoxs[i].clr.w);

            voxels->voxels[((int)tmpvoxs[i].position.y * (int)voxels->m_size.z + (int)tmpvoxs[i].position.z) * (int)voxels->m_size.x + (int)tmpvoxs[i].position.x].visible = true;
        }

        in.close();
        delete tmpvoxs;
        return voxels;
    } else {
        std::cerr << "ERROR::VOXMODEL::Voxel model not loaded: " << filename << std::endl;
        return nullptr;
    }
}