#include <iostream>
#include <glm/glm.hpp>
#include <unordered_map>
#include <functional>
#include <algorithm>

using namespace std;

#include <random>
#include <chrono>


int main() {
    auto start = std::chrono::high_resolution_clock::now();

    VoxelModel* voxelmodel = new VoxelModel();

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> coordDist(-500, 500);
    std::uniform_real_distribution<float> fillChance(0.0f, 1.0f);
    std::uniform_real_distribution<float> colorDist(0.0f, 1.0f);

    const float FILL_PROBABILITY = 0.1f; // 10% шанс, что воксель появится

    const int NUM_ATTEMPTS = 100000; // сколько ячеек пытаемся создать

    // Voxel* voxel = new Voxel();
    int min_x = std::numeric_limits<int>::max();
    int min_y = std::numeric_limits<int>::max();
    int min_z = std::numeric_limits<int>::max();

    int max_x = std::numeric_limits<int>::min();
    int max_y = std::numeric_limits<int>::min();
    int max_z = std::numeric_limits<int>::min(); 

    for (int i = 0; i < NUM_ATTEMPTS; ++i) {
        if (fillChance(rng) > FILL_PROBABILITY) continue; // пропускаем большинство клеток

        int x = coordDist(rng);
        int y = coordDist(rng);
        int z = coordDist(rng);
        
        min_x = (x < min_x) ? x : min_x;
        min_y = (y < min_y) ? y : min_y;
        min_z = (z < min_z) ? z : min_z;

        max_x = (x > max_x) ? x : max_x;
        max_y = (y > max_y) ? y : max_y;
        max_z = (z > max_z) ? z : max_z;

        Voxel v;
        v.position = glm::ivec3(x, y, z);
        v.color = glm::vec4(colorDist(rng), colorDist(rng), colorDist(rng), 1.0f);
        v.visible = true;

        voxelmodel->setVoxel(x, y, z, v);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";

    std::cout << min_x << " " << min_y << " " << min_z << " | " << max_x << " " << max_y << " " << max_z << "\n";

    // проверка
    Voxel* test = voxelmodel->getVoxel(0, 0, 0);
    if (test) {
        std::cout << "Voxel at 0,0,0 exists: color=("
                  << test->color.r << ", "
                  << test->color.g << ", "
                  << test->color.b << ")\n";
    } else {
        std::cout << "Voxel at 0,0,0 does not exist\n";
    }

    return 0;
}