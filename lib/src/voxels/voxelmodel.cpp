#include "voxelmodel.h"
#include <fstream>
#include <cstring>
#include <glm/gtc/type_ptr.hpp>
#include "../graphics/mesh.h"

#include <iostream>

VoxelModel::VoxelModel(glm::i16vec3 size, int depth) : _size(size) {
    _voxels = new vtype::array3<int32_t, Voxel*>(size.x, size.y, size.z);
}
VoxelModel::~VoxelModel() {}

void VoxelModel::setName(std::string name) {
    _name = name;
}
std::string VoxelModel::getName() {
    return _name;
}

void VoxelModel::addVoxel(Voxel *voxel) {
    glm::ivec3 pos = voxel->getPosition();
    _voxels->add(pos.x, pos.y, pos.z, voxel);
}
// Метод для удаления вокселя по координатам и освобождения памяти
void VoxelModel::removeVoxel(glm::ivec3 position) {
    delete _voxels->get(position.x, position.y, position.z);
    // _voxels->
}
void VoxelModel::setVoxelVisible(glm::ivec3 position, bool visible) { 
    Voxel* voxel = getVoxel(position);
    if (voxel != nullptr) {
        voxel->setVisible(visible); 
    }
}

Voxel* VoxelModel::getVoxel(glm::ivec3 position) {
    return _voxels->get(position.x, position.y, position.z);
}

void VoxelModel::fillColor(glm::vec4 color) {

}

void VoxelModel::setMesh(Mesh *mesh) {
    _pmesh = mesh->clone();
}

Mesh* VoxelModel::getMesh() {
    return _pmesh;
}

VoxelModel* load_model(std::string filename, const char* type) {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream in(filename);
    if (in.is_open()) {
        std::string line;
        uint16_t vi = 0;

        short x_coord, y_coord, z_coord;
        int x_min = 999; int y_min = 999; int z_min = 999;
        int x_max = -999; int y_max = -999; int z_max = -999;
        float clr_r = 0.0f, clr_g = 0.0f, clr_b = 0.0f, clr_a = 0.0f;

        std::string str[10];

        vi = 0;
        uint voxelcount = 0;

        Voxel** __voxels = new Voxel*[700000];

        while (getline(in, line)) {
            vi++;
            if ( vi < 5 ) continue;
            // std::cout << vi << " " << line << std::endl;
            
            split(str, line, ' '); // x y z clr

            x_coord = atoi(str[0].c_str());
            y_coord = atoi(str[1].c_str());
            z_coord = atoi(str[2].c_str());

            if (x_coord < 0 && x_coord < x_min) x_min = x_coord;
            if (y_coord < 0 && y_coord < y_min) y_min = y_coord;
            if (z_coord < 0 && z_coord < z_min) z_min = z_coord;
            
            //* VOXEL COLOR
            const char* clr = str[3].c_str();
            char r[2] = ""; char g[2] = "";
            strncat(r, clr, 2);
            clr_r = round( strtoul(r, NULL, 16) / 255.0 * 100 ) / 100;
            strncat(g, clr+2, 2);
            clr_g = round( strtoul(g, NULL, 16) / 255.0 * 100 ) / 100;
            clr_b = round( strtoul(clr+4, NULL, 16) / 255.0 * 100 ) / 100;
            clr_a = 1.0f;

            glm::ivec3 position = glm::vec3(x_coord, y_coord, z_coord);
            glm::vec4 color = glm::vec4(clr_r, clr_g, clr_b, clr_a);
            Voxel* voxel = new Voxel(position, color);
            
            __voxels[voxelcount] = voxel;
            voxelcount++;
        }

        if ( x_min == 999 ) x_min = 0;
        if ( y_min == 999 ) y_min = 0;
        if ( z_min == 999 ) z_min = 0;
        if ( x_max == -999 ) x_max = 0;
        if ( y_max == -999 ) y_max = 0;
        if ( z_max == -999 ) z_max = 0;

        for (size_t i = 0; i < voxelcount; i++) {
            glm::ivec3 position = __voxels[i]->getPosition();
            glm::vec4 color = __voxels[i]->getColor();

            int pos_x = position.x + abs(x_min);
            int pos_y = position.y + abs(y_min);
            int pos_z = position.z + abs(z_min);

            __voxels[i]->setPosition(glm::vec3(pos_x, pos_y, pos_z));

            if (pos_x > x_max) x_max = pos_x;
            if (pos_y > y_max) y_max = pos_y;
            if (pos_z > z_max) z_max = pos_z;
        }

        VoxelModel* voxels = new VoxelModel(glm::vec3(100));

        for (size_t i = 0; i < voxelcount; i++) {
            voxels->addVoxel(__voxels[i]);
            __voxels[i] = nullptr;
        }
        voxels->setSize(glm::vec3(x_max + 1, y_max + 1, z_max + 1));
        in.close();
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = end - start;

        Logger::eprint("LOADER", "Loaded model: " + std::string(CYAN_COLOR) + filename + "	" + std::string(BLUE_COLOR) + std::to_string(duration.count()) + "s" + std::string(RESET_COLOR),  LOGLEVEL::INFO);
        return voxels;
    } else {
        // Logger::eprint("LOADER", "", LOGLEVEL::ERROR)
        // std::cerr << "ERROR::VOXMODEL::Voxel model not loaded: " << filename << std::endl;
        return nullptr;
    }
}

VoxelModel* genVoxel() {
    VoxelModel* nullvox = new VoxelModel(glm::ivec3(1));
    Voxel* voxel = new Voxel(glm::vec3(0), glm::vec4(0));
    nullvox->addVoxel(voxel);
    return nullvox;
}