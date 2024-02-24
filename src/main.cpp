/**
 * @file main.cpp
 * @author Vlad Kartsaev
 * @brief Voxel3D Engine demo application
 * @version 2.0
 * @date 2023-04-06
 *
 * @copyright Copyright (c) 2024
 *
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
#include "graphics/textmesh.h"

#include "loaders/resourcemanager.h"
#include "gamesystems/gamemanager.h"

#include "voxels/voxelmodel.h"
#include "voxels/particlesmodel.h"
#include "gamesystems/camera.h"
#include "gamesystems/entity.h"

#include "system/taskscheduler.h"

#include "graphics/cube.h"

// #include <ft2build.h>

#include <random>

/**
 * @brief Версия Voxel3D Engine
 */
#define _VERSION "0.2.0"

/**
 * @brief Размера буфера рендера
 *
 */
#define _RENDERSIZE 4096 * 4096

/**
 * @brief Заголовок окна
 *
 */
#define TITLE "Voxel3D DEMO"

int WIDTH = 1024;
int HEIGHT = 768;
float MOUSE_SPEED = 1.3f;

#include <random>
#include <chrono>

std::mt19937 rng1(std::random_device{}());

int main() {
    printf("Voxel 3D: %s\n", _VERSION);
    Window::init(WIDTH, HEIGHT, TITLE);
    Input::init();
    ResourceManager::init("../res/");

    //! Global renderer
    Renderer::init(_RENDERSIZE);
    // std::cout << std::endl;

    ResourceManager::loadShader("voxel");
    ResourceManager::loadShader("particle");
    ResourceManager::loadShader("crosshair");
    ResourceManager::loadShader("ui");
    ResourceManager::loadShader("font");

    std::cout << std::endl;

    ResourceManager::loadTexture("slot");

    std::cout << std::endl;

    ResourceManager::loadModel("apple", "voxtxt"); //? voxtxt, generic, null, voxlap
    ResourceManager::loadModel("bread", "voxtxt");
    ResourceManager::loadModel("null", "null");
    
    std::cout << std::endl;

    ResourceManager::loadFont("arial");

    std::cout << std::endl;

    ResourceManager::loadVoxelParticles("testgravity");
    ResourceManager::loadVoxelParticles("what");

    GameManager *gm = new GameManager();
    // PhysicsEngine* physicsengine = gm->getPhysicsEngine();

    //! Camera
    Camera *camera = new Camera(vec3(3, 1, 0), radians(70.0f));
    Renderer::addCamera(camera);

    VoxelModel *floorvox = new VoxelModel(glm::ivec3(10, 1, 100), 10);

    for (int y = 0; y < floorvox->getSize().y; y++) {
        for (int z = 0; z < floorvox->getSize().z; z++) {
            for (int x = 0; x < floorvox->getSize().x; x++) {
                // std::cout << x << " " << y << " " << z << std::endl;
                Voxel *voxel = new Voxel(glm::vec3(x, y, z));
                voxel->setColor(vec4(0.3f, 0.3f, 0.5f, 1.0f));
                floorvox->addVoxel(voxel);
            }
        }
    }

    std::uniform_real_distribution<float> redclr_generator = std::uniform_real_distribution<float>(0.0f, 1.0f);
    std::uniform_real_distribution<float> greenclr_generator = std::uniform_real_distribution<float>(0.0f, 1.0f);
    std::uniform_real_distribution<float> blueclr_generator = std::uniform_real_distribution<float>(0.0f, 1.0f);

    VoxelModel *wallvox = new VoxelModel(glm::ivec3(1, 10, 50));
    
    for (int y1 = 0; y1 < wallvox->getSize().y; y1++) {
        for (int z1 = 0; z1 < wallvox->getSize().z; z1++) { 
            for (int x1 = 0; x1 < wallvox->getSize().x; x1++) {
                Voxel *voxel = new Voxel(glm::vec3(x1, y1, z1));
                voxel->setColor(vec4(redclr_generator(rng1), 0.0f, 0.3f, 1.0f));
                
                wallvox->addVoxel(voxel);
            }
        }
    }
    VoxelModel *boxvox = new VoxelModel(glm::ivec3(10, 10, 10));

    for (int y1 = 0; y1 < boxvox->getSize().y; y1++) {
        for (int z1 = 0; z1 < boxvox->getSize().z; z1++) { 
            for (int x1 = 0; x1 < boxvox->getSize().x; x1++) {
                Voxel *voxel = new Voxel(glm::vec3(x1, y1, z1));
                voxel->setColor(vec4(0.2, 0.0f, blueclr_generator(rng1), 1.0f));
                
                boxvox->addVoxel(voxel);
            }
        }
    }

    ResourceManager::addModel(floorvox, "floor");
    ResourceManager::addModel(wallvox, "wall");
    ResourceManager::addModel(boxvox, "box");
    
    ResourceManager::prepareModel("apple");
    ResourceManager::prepareModel("floor");
    ResourceManager::prepareModel("wall");
    ResourceManager::prepareModel("bread");
    ResourceManager::prepareModel("box");

    // ! GameObjects
    GameObject *floorobj = new GameObject("floor", vec3(-500, -1, -500));
    floorobj->setPhysics(STATIC_PHYSICS);

    GameObject *wallobj = new GameObject("wall", vec3(20, 0, -15));
    wallobj->setPhysics(STATIC_PHYSICS);

    GameObject *boxobj = new GameObject("box", vec3(5, 0, -15));
    boxobj->setPhysics(DYNAMIC_PHYSICS);

    GameObject *appleobj = new GameObject("apple", vec3(0, 50, 40));
    appleobj->attachCamera(camera, vec3(0, 15, 0)); //! Attach camera to appleobj, apple hidden
    appleobj->setPhysics(DYNAMIC_PHYSICS);
    appleobj->setVisible(true);

    GameObject *appleobj1 = new GameObject("apple", vec3(0, 10, 0));
    appleobj1->setPhysics(DYNAMIC_PHYSICS);

    GameObject *appleobj2 = new GameObject("apple", vec3(20, 30, 20));
    appleobj2->setPhysics(DYNAMIC_PHYSICS);

    GameObject *breadobj = new GameObject("bread", vec3(50, 30, 20));
    breadobj->setPhysics(DYNAMIC_PHYSICS);

    gm->addGameObject(floorobj);
    gm->addGameObject(wallobj);
    gm->addGameObject(boxobj);
    gm->addGameObject(appleobj);
    gm->addGameObject(appleobj1);
    gm->addGameObject(appleobj2);
    gm->addGameObject(breadobj);

    //! Particles
    VoxelParticles *effect1 = new VoxelParticles("testgravity", 10000, vec3(4.0f, 500.0f, 1.5f));
    gm->addVoxelParticles(effect1);

    VoxelParticles *effect2 = new VoxelParticles("what", 10000, vec3(4.0f, 15, -15));
    gm->addVoxelParticles(effect2);

    float deltaTime = 0.0f;
    auto lastTimePoint = std::chrono::system_clock::now();
    vec2 cam(0.0f, 0.0f);
    float speed = 50;
    float jumpforce = 200.0f;

    GUI gui;
    int bul_i = 0;
    GameObject** bullets = new GameObject*[1000];

    for (int i = 0; i < 1000; i++) {
        GameObject* obj = new GameObject("box", camera->getPosition());
        obj->setPhysics(DYNAMIC_PHYSICS);
        obj->setVisible(false);
        gm->addGameObject(obj);
        bullets[i] = obj;
    }

    GameObject** walls = new GameObject*[10000];

    for (int y = 0; y < 1; y++) {
        for (int x = 0; x < 500; x++) {
            GameObject* obj = new GameObject("box", glm::vec3(-500+x*5, y*5, 0));
            obj->setPhysics(STATIC_PHYSICS);
            obj->setVisible(true);
            gm->addGameObject(obj);
            // walls[i] = obj;
        }
    }

    /**
     * @brief FREETYPE TEST
     * 
    */

    FT_Face test = ResourceManager::getFont("arial");
    TextMesh* textMesh = new TextMesh(test);

    //! ===MAIN-LOOP===
    bool quit{false};
    Window::setPause(false);
    while (!quit) {
        auto newTimePoint = std::chrono::system_clock::now();
        auto dtMsec = std::chrono::duration_cast<std::chrono::milliseconds>(newTimePoint - lastTimePoint);
        lastTimePoint = newTimePoint;
        deltaTime = 0.001f * float(dtMsec.count());

        Input::processEvents(quit);

        if (Input::_cursor_locked) {
            cam.x += -Input::deltaX * MOUSE_SPEED / Window::height;
            cam.y += -Input::deltaY * MOUSE_SPEED / Window::height;
            if (cam.y < -radians(89.0f))
                cam.y = -radians(89.0f);

            if (cam.y > radians(89.0f))
                cam.y = radians(89.0f);

            camera->setRotation(mat4(1.0f));
            camera->rotate(cam, 0);
            appleobj->setRotation(cam.x, glm::vec3(0, 1, 0));
            camera->setPosition((appleobj->getPosition() + vec3(0, 15, 0)) / 10.0f);
        }
        glm::vec3 camera_front = camera->getFrontVector();
        glm::vec3 camera_right = camera->getRightVector();

        if (Input::jpressed(SDLK_ESCAPE)) quit = true;

        if (Input::jpressed(SDLK_p)) {
            Window::setPause(!Window::getPause());
            Input::toggleCursor();
        }

        if (!Window::getPause()) {
            if (Input::jpressed(SDLK_TAB))
                Input::toggleCursor();

            if (Input::pressed(SDLK_w))
                appleobj->setPosition(appleobj->getPosition() + vec3(camera_front.x, 0, camera_front.z) * deltaTime * speed);
            if (Input::pressed(SDLK_s))
                appleobj->setPosition(appleobj->getPosition() - vec3(camera_front.x, 0, camera_front.z) * deltaTime * speed);
            if (Input::pressed(SDLK_a))
                appleobj->setPosition(appleobj->getPosition() - vec3(camera_right.x, 0, camera_right.z) * deltaTime * speed);
            if (Input::pressed(SDLK_d))
                appleobj->setPosition(appleobj->getPosition() + vec3(camera_right.x, 0, camera_right.z) * deltaTime * speed);

            if (appleobj->getPhysicsObject()->isGrounded()) {
                if (Input::jpressed(SDLK_SPACE)) {
                    appleobj->getPhysicsObject()->applyForce(vec3(0, jumpforce * 5, 0));
                }
            }
            if (Input::pressed(SDLK_c)) {
                camera->setZoom(0.2f);
            } else {
                camera->setZoom(1.0f);
            }

            if (Input::jclicked(SDL_BUTTON_MIDDLE)) {
                appleobj1->getPhysicsObject()->applyForce(vec3(0, jumpforce * 5, -jumpforce * 5));
            }
            if (Input::jclicked(SDL_BUTTON_RIGHT)) {
                appleobj->setPosition(vec3(0, 100, 0));
            }
            if (Input::clicked(SDL_BUTTON_LEFT)) {
                if (bul_i > 999) {
                    bul_i = 0;
                }
                bullets[bul_i]->setVisible(true);
                // appleobj1->getPhysicsObject()->applyForce(vec3(0, jumpforce * 5, 0));
                bullets[bul_i]->getPhysicsObject()->stopForce();
                bullets[bul_i]->setPosition(camera->getPosition() + camera->getFrontVector() * 1.0f);
                bullets[bul_i]->getPhysicsObject()->applyForce(camera->getFrontVector() * 10500.0f);
                
                bul_i++;
            }
            appleobj->translate(1.0f, vec3(0, 1, 0));
        }

        glm::vec3 normalizedDirection = glm::normalize(camera_front);

        // Умножаем нормализованный вектор направления на длину вокселя, а затем на заданную длину,
        // чтобы получить конечную точку линии
        // glm::vec3 endPosition = camera->getPosition() + camera_front * 30.0f;

        // float x2 = (camera->getPosition().x + camera_front.x * 15.0f);
        // float y2 = (camera->getPosition().y + camera_front.y * 15.0f);
        // float z2 = (camera->getPosition().z + camera_front.z * 15.0f);
        
        // glm::vec3 endPosition = glm::vec3(x2, y2, z2); //glm::abs(appleobj1->getPosition() - glm::vec3(x2, y2, z2));
        
        // std::cout << "Camera: " << camera->getPosition().x << " " << camera->getPosition().y << " " << camera->getPosition().z << "\n";
        // // std::cout << "End: " << x2 << " " << y2 << " " << z2 << "\n";
        // std::cout << "Vect: " << endPosition.x << " " << endPosition.y << " " << endPosition.z << "\n";
        // std::cout << std::endl;
        // std::cout << "APPLEPOS: " << appleobj1->getPosition().x << " " << appleobj1->getPosition().y << " " << appleobj1->getPosition().z << "\n";

        // Voxel* vox = appleobj1->getVoxelModel()->getVoxel( endPosition );
        // if (vox != nullptr) {
        //     std::cout << "VOXEL: " << vox->getPosition().x << " " << vox->getPosition().y << " " << vox->getPosition().z << "\n";
        //     // std::cout << "Hit" << std::endl;
        // }
        

        // glm::vec3 test = -camera->getPosition() + camera->getFrontVector() * 5.0f;
        // std::cout << "Camera: " << camera->getPosition().x << " " << camera->getPosition().y << " " << camera->getPosition().z << std::endl;
        // // std::cout << "Front: " << camera->getFrontVector().x << " " << camera->getFrontVector().y << " " << camera->getFrontVector().z << std::endl;
        // std::cout << "Test: " << test.x << " " << test.y << " " << test.z << std::endl;

        Window::_glClear();

        ResourceManager::getShader("particle")->use();
        ResourceManager::getShader("particle")->uniformMatrix("projviewmodel", Renderer::getCamera()->getProjection() * Renderer::getCamera()->getView() * glm::scale(glm::mat4(1.0f), glm::vec3(0.05f)));
        // gm->UpdateParticles(deltaTime);
        // Texture *texture = new Texture();
        // texture->bind();

        ResourceManager::getShader("voxel")->use();
        ResourceManager::getShader("voxel")->uniformMatrix("projview", Renderer::getCamera()->getProjection() * Renderer::getCamera()->getView());

        gm->UpdatePhysics(deltaTime);
        gm->Update();

        ResourceManager::getShader("font")->use();
        ResourceManager::getShader("font")->uniformVec3("textColor", glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
        ResourceManager::getShader("font")->uniformMatrix("projection", projection);
        textMesh->draw("TEST C++", 50.0f, 25.0f, 1.0f);

        // gui.draw();

        Window::swapBuffers();
        Input::pullEvents();
    }
    Input::cleanup();
    Window::exit();
    return 0;
}