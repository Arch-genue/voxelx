#include "voxelmodel.h"
#include <fstream>
#include <cstring>
#include <glm/gtc/type_ptr.hpp>
#include "../graphics/mesh.h"

#include <iostream>

class Octree;

VoxelModel::VoxelModel(glm::vec3 size, int depth) : _size(size) {
    _voxels = new Octree(glm::vec3(0), glm::vec3(_size.x, _size.y, _size.z), depth);
}
VoxelModel::~VoxelModel() {
    delete _voxels;
}

void VoxelModel::setName(std::string name) {
    _name = name;
}
std::string VoxelModel::getName() {
    return _name;
}


void VoxelModel::addVoxel(Voxel *voxel) {
    _voxels->insert(voxel);

}
// Метод для удаления вокселя по координатам и освобождения памяти
void VoxelModel::removeVoxel(glm::vec3 position) {
    Voxel* vox = _voxels->find(position);
    if (vox != nullptr) {
        _voxels->remove(_voxels->find(position));
    }
}
void VoxelModel::setVoxelVisible(glm::ivec3 position, bool visible) { 
    Voxel* voxel = getVoxel(position);
    
    if (voxel != nullptr) {
        voxel->setVisible(visible); 
    }
}

Voxel* VoxelModel::getVoxel(glm::ivec3 position) {
    return _voxels->find(position);
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
        // delete __voxels;
        in.close();
        return voxels;
    } else {
        std::cerr << "ERROR::VOXMODEL::Voxel model not loaded: " << filename << std::endl;
        return nullptr;
    }
}

VoxelModel* genVoxel() {
    VoxelModel* nullvox = new VoxelModel(glm::ivec3(1));
    Voxel* voxel = new Voxel(glm::vec3(0), glm::vec4(0));
    nullvox->addVoxel(voxel);
    return nullvox;
}

Octree::Octree(const glm::vec3 &min, const glm::vec3 &max, int maxDepth) 
    : _root(nullptr), _minBounds(min), _maxBounds(max), _maxDepth(maxDepth) {
    _root = new OctreeNode(); // Создаем корневой узел
}

void Octree::insert(Voxel *voxel) {
    insertRecursive(_root, voxel, _minBounds, _maxBounds, 0);
}

void Octree::insertRecursive(OctreeNode* node, Voxel* voxel, const glm::vec3& min, const glm::vec3& max, int depth) {
    // Проверяем, находится ли воксель в пределах узла
    if (voxel->getPosition().x < min.x || voxel->getPosition().x >= max.x ||
        voxel->getPosition().y < min.y || voxel->getPosition().y >= max.y ||
        voxel->getPosition().z < min.z || voxel->getPosition().z >= max.z) {
        return;
    }

    // Если достигнута максимальная глубина или узел является листовым, добавляем воксель в узел
    if (depth == _maxDepth || !node->children[0]) {
        node->voxels.push_back(voxel);
        return;
    }

    // Разделяем узел, если это необходимо
    glm::vec3 mid = (min + max) * 0.5f;
    int childIndex = 0;
    if (voxel->getPosition().x >= mid.x) childIndex |= 1;
    if (voxel->getPosition().y >= mid.y) childIndex |= 2;
    if (voxel->getPosition().z >= mid.z) childIndex |= 4;

    if (!node->children[childIndex]) {
        // Создаем дочерний узел, если он еще не создан
        node->children[childIndex] = new OctreeNode();
        // Вычисляем границы дочернего узла
        glm::vec3 childMin, childMax;
        childMin.x = (childIndex & 1) ? mid.x : min.x;
        childMax.x = (childIndex & 1) ? max.x : mid.x;
        childMin.y = (childIndex & 2) ? mid.y : min.y;
        childMax.y = (childIndex & 2) ? max.y : mid.y;
        childMin.z = (childIndex & 4) ? mid.z : min.z;
        childMax.z = (childIndex & 4) ? max.z : mid.z;
        // Рекурсивно вставляем воксель в дочерний узел
        insertRecursive(node->children[childIndex], voxel, childMin, childMax, depth + 1);
    } else {
        // Рекурсивно вставляем воксель в существующий дочерний узел
        insertRecursive(node->children[childIndex], voxel, mid, max, depth + 1);
    }
}

void Octree::remove(Voxel* voxel) {
    removeRecursive(_root, voxel, _minBounds, _maxBounds, 0);
}

void Octree::removeRecursive(OctreeNode* node, Voxel* voxel, const glm::vec3& min, const glm::vec3& max, int depth) {
    // Проверяем, находится ли воксель в пределах узла
    if (voxel->getPosition().x < min.x || voxel->getPosition().x >= max.x ||
        voxel->getPosition().y < min.y || voxel->getPosition().y >= max.y ||
        voxel->getPosition().z < min.z || voxel->getPosition().z >= max.z) {
        return;
    }

    // Если достигнута максимальная глубина или узел является листовым, удаляем воксель из узла
    if (depth == _maxDepth || !node->children[0]) {
        for (auto it = node->voxels.begin(); it != node->voxels.end(); ++it) {
            if (*it == voxel) {
                node->voxels.erase(it);
                break;
            }
        }
        return;
    }

    // Разделяем узел, если это необходимо
    glm::vec3 mid = (min + max) * 0.5f;
    int childIndex = 0;
    if (voxel->getPosition().x >= mid.x) childIndex |= 1;
    if (voxel->getPosition().y >= mid.y) childIndex |= 2;
    if (voxel->getPosition().z >= mid.z) childIndex |= 4;

    if (node->children[childIndex]) {
        removeRecursive(node->children[childIndex], voxel, mid, max, depth + 1);
    }
}

Voxel* Octree::find(const glm::vec3& position) {
    return findRecursive(_root, position);
}

Voxel* Octree::findRecursive(OctreeNode* node, const glm::vec3& position) {
    // Если узел пуст или позиция находится за пределами границ узла, возвращаем nullptr
    if (!node || position.x < _minBounds.x || position.x > _maxBounds.x ||
        position.y < _minBounds.y || position.y > _maxBounds.y ||
        position.z < _minBounds.z || position.z > _maxBounds.z) {
        return nullptr;
    }

    // Если узел является листовым, проверяем воксели в нем
    if (node->voxels.size() > 0) {
        // Проверяем каждый воксель в узле на точное соответствие позиции
        for (Voxel* voxel : node->voxels) {
            if (glm::all(glm::equal(voxel->getPosition(), position, glm::epsilon<float>()))) {
                return voxel;  // Найден воксель, возвращаем его
            }
        }
        return nullptr;  // Воксель не найден в узле
    }

    // Рекурсивно ищем воксель в дочерних узлах, в которых находится позиция
    glm::vec3 mid = (_minBounds + _maxBounds) * 0.5f;
    int childIndex = 0;
    if (position.x >= mid.x) childIndex |= 1;
    if (position.y >= mid.y) childIndex |= 2;
    if (position.z >= mid.z) childIndex |= 4;

    return findRecursive(node->children[childIndex], position);
}