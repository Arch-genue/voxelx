/*
Voxel3D Engine 2023


C++, OpenGL
*/

#include <iostream>
#include <chrono>

#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
using namespace glm;

#include "window/newwindow.h"
#include "window/newinput.h"

#include "graphics/renderer.h"
#include "graphics/gui.h"

#include "loaders/resourcemanager.h"
#include "gamesystems/gamemanager.h"

#include "voxels/voxel.h"
#include "gamesystems/camera.h"
#include "gamesystems/entity.h"

#define _VERSION "0.2.0"

#define _RENDERERSIZE 1024*1024

#define TITLE "Voxel3D DEMO"

int WIDTH = 1024;
int HEIGHT = 768;
float MOUSE_SPEED = 1.3f;

#include <algorithm>

struct AABBD {
    std::string name;
    float min_x, min_y, min_z;
    float max_x, max_y, max_z;
};

// // Функция для проверки пересечения двух AABB
// bool intersect(const AABB& a, const AABB& b) {
//     // Проверяем, не пересекаются ли AABB по каждой оси
//     bool x_overlap = (a.min_x <= b.max_x) && (a.max_x >= b.min_x);
//     bool y_overlap = (a.min_y <= b.max_y) && (a.max_y >= b.min_y);
//     bool z_overlap = (a.min_z <= b.max_z) && (a.max_z >= b.min_z);

//     // AABB пересекаются, если они пересекаются по каждой оси
//     return x_overlap && y_overlap && z_overlap;
// }

// // Функция для проверки столкновений между объектами с использованием AABB
// void checkCollisions(const std::vector<AABB>& objects) {
//     // Перебираем все пары объектов и проверяем их на столкновения
//     for (size_t i = 0; i < objects.size(); ++i) {
//         for (size_t j = i + 1; j < objects.size(); ++j) {
//             // Проверяем столкновение между объектами objects[i] и objects[j]
//             if (intersect(objects[i], objects[j])) {
//                 std::cout << "Collision detected between objects " << i << " and " << j << std::endl;
//             }
//         }
//     }
// }

// Класс для узла дерева BVH
class BVHNode {
public:
    AABBD bounds;
    BVHNode* left;
    BVHNode* right;

    // Конструктор
    BVHNode(const AABBD& bounds) : bounds(bounds), left(nullptr), right(nullptr) {}
};

// Функция для вычисления ограничивающего объема для группы объектов
AABBD calculateBounds(const std::vector<AABBD>& objects) {
    // Инициализируем границы минимальными и максимальными значениями
    AABBD bounds;
    bounds.min_x = bounds.min_y = bounds.min_z = std::numeric_limits<float>::max();
    bounds.max_x = bounds.max_y = bounds.max_z = std::numeric_limits<float>::lowest();

    // Находим минимальные и максимальные координаты объектов
    for (const auto& object : objects) {
        // Обновляем границы
        bounds.min_x = std::min(bounds.min_x, object.min_x);
        bounds.min_y = std::min(bounds.min_y, object.min_y);
        bounds.min_z = std::min(bounds.min_z, object.min_z);
        bounds.max_x = std::max(bounds.max_x, object.max_x);
        bounds.max_y = std::max(bounds.max_y, object.max_y);
        bounds.max_z = std::max(bounds.max_z, object.max_z);
    }

    return bounds;
}

// Функция для проверки пересечения двух AABB
bool intersect(const AABBD& a, const AABBD& b) {
    // Проверяем, не пересекаются ли AABB по каждой оси
    bool x_overlap = (a.min_x <= b.max_x) && (a.max_x >= b.min_x);
    bool y_overlap = (a.min_y <= b.max_y) && (a.max_y >= b.min_y);
    bool z_overlap = (a.min_z <= b.max_z) && (a.max_z >= b.min_z);

    // AABB пересекаются, если они пересекаются по каждой оси
    return x_overlap && y_overlap && z_overlap;
}


// Функция для построения дерева BVH
BVHNode* buildBVHTree(const std::vector<AABBD>& objects) {
    // Если нет объектов, вернуть nullptr
    if (objects.empty()) {
        return nullptr;
    }

    // Создаем новый узел дерева
    BVHNode* node = new BVHNode(calculateBounds(objects));

    // Если вектор содержит только один объект, этот объект будет листом дерева
    if (objects.size() == 1) {
        node->left = node->right = nullptr;
    } else {
        // Разбиваем группу объектов на две равные части
        std::vector<AABBD> left_objects(objects.begin(), objects.begin() + objects.size() / 2);
        std::vector<AABBD> right_objects(objects.begin() + objects.size() / 2, objects.end());

        // Рекурсивно строим левое и правое поддеревья
        node->left = buildBVHTree(left_objects);
        node->right = buildBVHTree(right_objects);
    }

    return node;
}

// Функция для проверки столкновений с использованием дерева BVH
void checkCollisionsWithBVH(const BVHNode* node, const AABBD& object, std::vector<AABBD>& colliding_objects) {
    if (node == nullptr) {
        return;
    }

    // Если объект не пересекает ограничивающий объем узла, пропустить этот узел
    if (!intersect(node->bounds, object)) {
        return;
    }

    // Если узел - лист, проверяем столкновения с объектом
    if (node->left == nullptr && node->right == nullptr) {
        // Проверяем столкновение с объектами в этом листе
        if (intersect(node->bounds, object)) {
            colliding_objects.push_back(node->bounds);
        }
    } else {
        // Рекурсивно проверяем столкновения с дочерними узлами
        checkCollisionsWithBVH(node->left, object, colliding_objects);
        checkCollisionsWithBVH(node->right, object, colliding_objects);
    }
}


int main() {
    std::cout << "Voxel 3D: " << _VERSION << std::endl;
    Window::init(WIDTH, HEIGHT, TITLE);
    Input::init();
    ResourceManager::init("../res/");

    //! Global renderer
    Renderer::init(_RENDERERSIZE);
    std::cout << "Inititialization..." << std::endl;
    std::cout << std::endl;

    ResourceManager::loadShader("voxel");
    ResourceManager::loadShader("particle");
    ResourceManager::loadShader("crosshair");
    ResourceManager::loadShader("ui");

    std::cout << std::endl;

    ResourceManager::loadTexture("slot");

    std::cout << std::endl;

    ResourceManager::loadModel("apple", "voxtxt"); //? voxtxt, generic, null, voxlap
    ResourceManager::loadModel("null", "null");

    std::cout << std::endl;

    ResourceManager::loadVoxelParticles("testgravity");

    GameManager* gm = new GameManager();

    VoxelModel* floorvox = new VoxelModel;
    for(uint16_t i = 0; i < 6; i++) floorvox->setLight(i, glm::vec3(1));
    floorvox->setSize(vec3(1000, 1, 1000));
    floorvox->setRenderSide("top");
    for (size_t y = 0; y < floorvox->getSize().y; y++) {
        for (size_t z = 0; z < floorvox->getSize().z; z++) {
            for (size_t x = 0; x < floorvox->getSize().x; x++) {
                Voxel* voxel = new Voxel(glm::vec3(x, y, z));
                // voxel->setPosition(vec3(x, y, z));
                voxel->setColor(vec4(0.3f, 0.3f, 0.3f, 1.0f));
                floorvox->addVoxel(voxel);
            }
        }
    }

    ResourceManager::addModel(floorvox, "floor");

    //! GameObjects
    GameObject* floorobj = new GameObject("floor");
    floorobj->setPosition(vec3(-500, -1, -500));
    floorobj->setRigidBody(false);
    floorobj->setCollision(SIMPLE_COLLISION);

    GameObject* nullobj = new GameObject("null");
    nullobj->setPosition(vec3(15,0,0));
    
    GameObject* appleobj = new GameObject("apple");
    appleobj->setPosition(vec3(0, 50, 40));
    appleobj->setRigidBody(true);
    appleobj->setCollision(SIMPLE_COLLISION);
    // appleobj->setHidden(true);

    GameObject* appleobj1 = new GameObject("apple");
    appleobj1->setPosition(vec3(0, 10, 0));
    appleobj1->setRigidBody(true);
    appleobj1->setCollision(SIMPLE_COLLISION);

    gm->addGameObject(floorobj);
    gm->addGameObject(appleobj);
    gm->addGameObject(appleobj1);

    //! Camera
    Camera* camera = new Camera(vec3(3, 1, 0), radians(70.0f)); 
    Renderer::addCamera(camera);
    appleobj->attachCamera(camera, vec3(0, 15, 0)); //! Attach camera to appleobj, apple hidden

    //! Particles
    VoxelParticles* effect1 = new VoxelParticles("testgravity", 10000, vec3(4.0f, 500.0f, 1.5f));
    gm->addVoxelParticles(effect1);

    float deltaTime = 0.0f;
    auto lastTimePoint = std::chrono::system_clock::now();
    vec2 cam(0.0f, 0.0f);
    float speed = 50;
    float jumpforce = 200.0f;
    
    GUI gui;
    //! ===MAIN-LOOP===

    std::vector<AABBD> objects;
    AABBD object1 = {"Obj1", 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f}; // AABB с координатами (0,0,0) и размерами 1x1x1
    AABBD object2 = {"Obj2", -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f}; // AABB с координатами (-1,-1,-1) и размерами 1x1x1
    AABBD object3 = {"Obj3", 2.0f, 2.0f, 2.0f, 3.0f, 3.0f, 3.0f}; // AABB с координатами (2,2,2) и размерами 1x1x1
    objects.push_back(object1);
    objects.push_back(object2);
    objects.push_back(object3);

    // Строим дерево BVH для оптимизации проверки столкновений
    BVHNode* root = buildBVHTree(objects);

    // Проверяем столкновения с использованием дерева BVH для каждого объекта
    for (const auto& object : objects) {
        std::vector<AABBD> colliding_objects;
        checkCollisionsWithBVH(root, object, colliding_objects);

        // Выводим результаты проверки столкновений
        std::cout << colliding_objects.size() << std::endl;
        for (int i = 0; i < colliding_objects.size(); i++) {
            std::cout << "Collision detected between objects " << object.name << " and " << colliding_objects[i].name << std::endl;
        }
    }


    bool quit {false};
    Window::setPause(false);
	while (!quit) {
        auto newTimePoint = std::chrono::system_clock::now();
        auto dtMsec = std::chrono::duration_cast<std::chrono::milliseconds>(newTimePoint - lastTimePoint);
        lastTimePoint = newTimePoint;
        deltaTime = 0.001f * float(dtMsec.count());
        
		Input::processEvents(quit);

        if (Input::_cursor_locked) {
            cam.x += -Input::deltaX * MOUSE_SPEED / Window::height; cam.y += -Input::deltaY * MOUSE_SPEED / Window::height;
            if (cam.y < -radians(89.0f)) cam.y = -radians(89.0f);
            if (cam.y > radians(89.0f)) cam.y = radians(89.0f);

            camera->setRotation(mat4(1.0f));
            camera->rotate(cam, 0);
        }
        glm::vec3 camera_front = camera->getFrontVector(); 
        glm::vec3 camera_right = camera->getRightVector(); 
        if (Input::jpressed(SDLK_ESCAPE)) quit = true;
        if (Input::jpressed(SDLK_p)) {
            Window::setPause(!Window::getPause());
            Input::toggleCursor();
        }
        if (!Window::getPause()) {
            if (Input::jpressed(SDLK_TAB)) Input::toggleCursor();
            
            if (Input::pressed(SDLK_w)) appleobj->setPosition( appleobj->getPosition() + vec3(camera_front.x, 0, camera_front.z) * deltaTime * speed );
            if (Input::pressed(SDLK_s)) appleobj->setPosition( appleobj->getPosition() - vec3(camera_front.x, 0, camera_front.z) * deltaTime * speed );
            if (Input::pressed(SDLK_a)) appleobj->setPosition( appleobj->getPosition() - vec3(camera_right.x, 0, camera_right.z) * deltaTime * speed );
            if (Input::pressed(SDLK_d)) appleobj->setPosition( appleobj->getPosition() + vec3(camera_right.x, 0, camera_right.z) * deltaTime * speed );
            
            if (appleobj->getPhysics()->ground) {
                if (Input::jpressed(SDLK_SPACE)) appleobj->getPhysics()->applyForce(vec3(0, jumpforce*5, 0));
            }

            if (Input::pressed(SDLK_c)) {
                camera->setZoom(0.2f);
            } else {
                camera->setZoom(1.0f);
            }

            if (Input::jclicked(SDL_BUTTON_RIGHT)) {
                appleobj->setPosition(vec3(0, 100, 0));
            }
            if (Input::jclicked(SDL_BUTTON_LEFT)) {
                appleobj1->getPhysics()->applyForce(vec3(0, jumpforce*5, 0));
                // appleobj1->getPhysics()->explode(5);
                // gm->clearParticles();
            }
                //appleobj->translate(1.0f, vec3(0, 1, 0));
                vec3 end; vec3 norm; vec3 iend;
                if (appleobj1->raycast(camera->getPosition(), camera_front, 20.0f, end, norm, iend) ) {
                    // appleobj1->setBorder(true);
                    //appleobj1->setVisible(false);
                    std::cout << "see" << std::endl;
                }
                //std::cout << "ddd" << std::endl;
            //}
        }

        Window::_glClear();
        
        ResourceManager::getShader("voxel")->use();
        ResourceManager::getShader("voxel")->uniformMatrix("projview", Renderer::getCamera()->getProjection() * Renderer::getCamera()->getView());

        gm->UpdatePhysics(deltaTime);
        gm->Update();

        ResourceManager::getShader("particle")->use();
        ResourceManager::getShader("particle")->uniformMatrix("projviewmodel", Renderer::getCamera()->getProjection() * Renderer::getCamera()->getView() * glm::scale(glm::mat4(1.0f), glm::vec3(0.05f)));
        gm->UpdateParticles(deltaTime);

        camera->setPosition( (appleobj->getPosition() + vec3(0, 15, 0)) / 10.0f );        

        gui.draw();
        
	    Window::swapBuffers();
        Input::pullEvents();
	}
    Window::exit();

    return 0;
}