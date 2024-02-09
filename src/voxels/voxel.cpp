#include "voxel.h"

#include <iostream>
#include <fstream>
#include <cstring>

Voxel::Voxel() {
    setPosition(glm::vec3(0));
    setVelocity(glm::vec3(0));
    setColor(glm::vec4(1));
    setVisible(true);
}
Voxel::Voxel(glm::vec3 position): _position(position) {
    setVelocity(glm::vec3(0));
    setColor(glm::vec4(1));
    setVisible(true);
}
Voxel::Voxel(glm::vec3 position, glm::vec4 color): _position(position), _color(color) {
    setVelocity(glm::vec3(0));
    setVisible(true);
}
Voxel::Voxel(glm::vec3 position, glm::vec3 velocity, glm::vec4 color): _position(position), _velocity(velocity), _color(color) {
    setVisible(true);
}
Voxel::~Voxel() {}


/**
 * @brief Set the position of the Voxel.
 * 
 * @param position The new position of the Voxel.
 */
void Voxel::setPosition(glm::vec3 position) {
    _position = position;
}
/**
 * @brief Get the position of the Voxel
 * 
 * @return glm::vec3 
 */
glm::vec3 Voxel::getPosition() {
    return  _position;
}


/**
 * @brief Sets the velocity of the Voxel.
 * 
 * @param velocity The new velocity of the Voxel.
 */
void Voxel::setVelocity(glm::vec3 velocity) {
    _velocity = velocity;
}
/**
 * @brief Returns the velocity of the Voxel.
 * 
 * @return The velocity of the Voxel.
 */
glm::vec3 Voxel::getVelocity() {
    return _velocity;
}

void Voxel::setColor(glm::vec4 color) {
    _color = color;
}
glm::vec4 Voxel::getColor() {
    return _color;
}


void Voxel::setVisible(bool visible) {
    _visible = visible;
}
bool Voxel::isVisible() {
    return _visible;
}


void Voxel::setLifeTime(float lifetime) {
    _lifetime = lifetime;
}
float Voxel::getLifeTime() {
    return _lifetime;
}


VoxelModel::VoxelModel() {}
VoxelModel::~VoxelModel() {}


void VoxelModel::setLight(uint8_t side, glm::vec3 light) {
    if (side > 5) return;
    _light[side] = light;
}
glm::vec3 VoxelModel::getLight(uint8_t side) {
    if (side > 5) return glm::vec3(0);
    return _light[side];
}
glm::vec3* VoxelModel::getLightArray() {
    return _light;
}


void VoxelModel::addVoxel(Voxel* voxel) {
    _voxels.push_back(voxel);
}


int VoxelModel::getVoxelsCount() {
    return _voxels.size();
}


void VoxelModel::setVoxel(int num, Voxel* voxel) {
    _voxels[num] = voxel;
}
Voxel* VoxelModel::getVoxel(int num) {
    return _voxels[num];
}


void VoxelModel::setSize(glm::vec3 size) {
    _size = size;
}
glm::vec3 VoxelModel::getSize() {
    return _size;
}


void VoxelModel::setRenderSide(std::string renderside) {
    _renderside = renderside;
}
std::string VoxelModel::getRenderSide() {
    return _renderside;
}


void VoxelModel::setVoxelPosition(int num, glm::vec3 position) {
    _voxels[num]->setPosition(position);
}
glm::vec3 VoxelModel::getVoxelPosition(int num) {
    return _voxels[num]->getPosition();
}



VoxelModel* load_model(std::string filename, const char* type) {
    std::ifstream in(filename);
    if (in.is_open()) {
        std::string line;
        uint16_t vi = 0;

        short x_coord, y_coord, z_coord;
        int x_min = 999; int y_min = 999; int z_min = 999;
        int x_max = -999; int y_max = -999; int z_max = -999;
        float clr_r = 0.0f, clr_g = 0.0f, clr_b = 0.0f, clr_a = 0.0f;
        
        VoxelModel* voxels = new VoxelModel;
        for(uint16_t i = 0; i < 6; i++) voxels->setLight(i, glm::vec3(1));

        std::string str[5];

        vi = 0;
        while (getline(in, line)) {
            vi++;
            if ( vi < 5 ) continue;
            
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

            // vox.setVisibletrue;
            glm::vec3 position = glm::vec3(x_coord, y_coord, z_coord);
            glm::vec4 color = glm::vec4(clr_r, clr_g, clr_b, clr_a);
            Voxel* voxel = new Voxel(position, color);
            voxels->addVoxel(voxel);
            // voxels->voxels.push_back(vox);
        }
        if ( x_min == 999 ) x_min = 0;
        if ( y_min == 999 ) y_min = 0;
        if ( z_min == 999 ) z_min = 0;
        if ( x_max == -999 ) x_max = 0;
        if ( y_max == -999 ) y_max = 0;
        if ( z_max == -999 ) z_max = 0;

        for (size_t i = 0; i < voxels->getVoxelsCount(); i++) {
            float pos_x = voxels->getVoxelPosition(i).x + abs(x_min);
            float pos_y = voxels->getVoxelPosition(i).y + abs(y_min);
            float pos_z = voxels->getVoxelPosition(i).z + abs(z_min); 
            voxels->setVoxelPosition(i, glm::vec3(pos_x, pos_y, pos_z));

            // pos_x = voxels->getVoxelPosition(i).x;
            // pos_y = voxels->getVoxelPosition(i).y;
            // pos_z = voxels->getVoxelPosition(i).z;
            if (pos_x > x_max) x_max = pos_x;
            if (pos_y > y_max) y_max = pos_y;
            if (pos_z > z_max) z_max = pos_z;
        }

        voxels->setSize(glm::vec3(x_max + 1, y_max + 1, z_max + 1)); //m_size.x = x_max+1;

        in.close();
        return voxels;
    } else {
        std::cerr << "ERROR::VOXMODEL::Voxel model not loaded: " << filename << std::endl;
        return nullptr;
    }
}

VoxelModel* genVoxel() {
    VoxelModel* nullvox = new VoxelModel;
    nullvox->setSize(glm::vec3(1, 1, 1));
    Voxel* voxel = new Voxel(glm::vec3(0), glm::vec4(0));
    nullvox->addVoxel(voxel);
    return nullvox;
}