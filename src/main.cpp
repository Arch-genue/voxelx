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

#include "window/window.h"
#include "window/input.h"

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

bool DEBUG_MODE = true;

#include <random>
#include <chrono>

std::mt19937 rng1(std::random_device{}());

int main() {
    printf("Voxel 3D: %s\n\n", _VERSION);
    std::string _version = _VERSION;

    Window::init(WIDTH, HEIGHT, TITLE);
    Input::init();
    ResourceManager::init("../res/");
    
    std::cout << std::endl;

    //! Global renderer
    Renderer::init(_RENDERSIZE);
    
    std::cout << std::endl;

    ResourceManager::loadShader("voxel");
    ResourceManager::loadShader("light");
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

    ResourceManager::loadVoxelParticles("testgravity");
    ResourceManager::loadVoxelParticles("what");
    
    std::cout << std::endl;
    
    ResourceManager::loadFont("arial");

    std::cout << std::endl;

    GameManager *gm = new GameManager();
    
    std::cout << std::endl;
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

    VoxelModel *lightvox = new VoxelModel(glm::ivec3(5, 5, 5));

    for (int y1 = 0; y1 < lightvox->getSize().y; y1++) {
        for (int z1 = 0; z1 < lightvox->getSize().z; z1++) { 
            for (int x1 = 0; x1 < lightvox->getSize().x; x1++) {
                Voxel *voxel = new Voxel(glm::vec3(x1, y1, z1));
                voxel->setColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
                
                lightvox->addVoxel(voxel);
            }
        }
    }

    ResourceManager::addModel(floorvox, "floor");
    ResourceManager::addModel(wallvox, "wall");
    ResourceManager::addModel(boxvox, "box");
    ResourceManager::addModel(lightvox, "light");
    
    ResourceManager::prepareModel("apple");
    ResourceManager::prepareModel("floor");
    ResourceManager::prepareModel("wall");
    ResourceManager::prepareModel("bread");
    ResourceManager::prepareModel("box");
    ResourceManager::prepareModel("light");

    // ! GameObjects
    // GameObject *floorobj = new GameObject("floor", vec3(-500, -1, -500));
    // floorobj->setPhysics(STATIC_PHYSICS);

    // GameObject *wallobj = new GameObject("wall", vec3(20, 0, -15));
    // wallobj->setPhysics(STATIC_PHYSICS);

    GameObject *boxobj = new GameObject("box", vec3(0, 0, -0));
    boxobj->setPhysics(DYNAMIC_PHYSICS);

    GameObject *lightobj = new GameObject("light", vec3(-15, 5, -15));
    lightobj->setPhysics(STATIC_PHYSICS);

    GameObject *appleobj = new GameObject("apple", vec3(0, 50, 40));
    appleobj->attachCamera(camera, vec3(0, 15, 0)); //! Attach camera to appleobj, apple hidden
    appleobj->setPhysics(DYNAMIC_PHYSICS);
    appleobj->setVisible(false);

    // GameObject *appleobj1 = new GameObject("apple", vec3(0, 10, 0));
    // appleobj1->setPhysics(DYNAMIC_PHYSICS);

    // GameObject *appleobj2 = new GameObject("apple", vec3(20, 30, 20));
    // appleobj2->setPhysics(DYNAMIC_PHYSICS);

    // GameObject *breadobj = new GameObject("bread", vec3(50, 30, 20));
    // breadobj->setPhysics(DYNAMIC_PHYSICS);

    // gm->addGameObject(floorobj);
    // gm->addGameObject(wallobj);
    gm->addGameObject(boxobj);
    gm->addGameObject(lightobj);
    gm->addGameObject(appleobj);
    // gm->addGameObject(appleobj1);
    // gm->addGameObject(appleobj2);
    // gm->addGameObject(breadobj);

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

        if (Input::getCursorLock()) {
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

        if (Input::jpressed(SDLK_F3)) DEBUG_MODE = !DEBUG_MODE;
        if (Input::jpressed(SDLK_F11)) Window::toggleFullscreen();

        if (Input::jpressed(SDLK_p)) {
            Window::setPause(!Window::getPause());
            Input::toggleCursor();
        }

        if (!Window::getPause()) {
            if (Input::jpressed(SDLK_TAB)) Input::toggleCursor();

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

            // if (Input::jclicked(SDL_BUTTON_MIDDLE)) {
            //     appleobj1->getPhysicsObject()->applyForce(vec3(0, jumpforce * 5, -jumpforce * 5));
            // }
            if (Input::jclicked(SDL_BUTTON_RIGHT)) {
                appleobj->setPosition(vec3(0, 100, 0));
            }

            appleobj->translate(1.0f, vec3(0, 1, 0));
        }

        Window::_glClear();

        // ResourceManager::getShader("particle")->use();
        // ResourceManager::getShader("particle")->uniformMatrix("projviewmodel", Renderer::getCamera()->getProjection() * Renderer::getCamera()->getView() * glm::scale(glm::mat4(1.0f), glm::vec3(0.05f)));
        // gm->UpdateParticles(deltaTime);

        Light light;
        light.position = camera->getPosition();
        light.direction = camera->getFrontVector();
        light.cutOff = glm::cos(glm::radians(15.0f));
        light.outerCutOff = glm::cos(glm::radians(30.0f));
        
        light.ambient = glm::vec3(0.1f);
        light.diffuse = glm::vec3(0.8f);
        light.specular = glm::vec3(1.0f);
        light.constant = 0.1f;
        light.linear = 0.014f;
        light.quadratic = 0.00007f;

        ResourceManager::getShader("voxel")->use();
        
        // //! LIGHT TEST
        ResourceManager::getShader("voxel")->uniformMatrix("projection", Renderer::getCamera()->getProjection());
        ResourceManager::getShader("voxel")->uniformMatrix("view", Renderer::getCamera()->getView());
        // ResourceManager::getShader("voxel")->uniformMatrix("model", glm::scale(glm::mat4(1.0f), glm::vec3(0.1f)));
        
        ResourceManager::getShader("voxel")->uniformVec3("viewPos", camera->getPosition());
                
        gm->UpdatePhysics(deltaTime);
        gm->Update(light);
        // gui.draw();

        //? TEXT
        ResourceManager::getShader("font")->use();
        glm::mat4 projection = glm::ortho(0.0f, (float)Window::width, 0.0f, (float)Window::height);
        ResourceManager::getShader("font")->uniformMatrix("projection", projection);
        // std::string text( + _VERSION);

        ResourceManager::getShader("font")->uniformVec3("textColor", glm::vec3(0.1f, 0.1f, 0.1f));
        textMesh->draw("Voxel 3D: " + _version, 5.0f, (float)Window::height-20.0f, 0.4f);

        if (DEBUG_MODE) {
            ResourceManager::getShader("font")->uniformVec3("textColor", glm::vec3(0.2f, 0.5f, 0.2f));
            textMesh->draw("Debug mode", Window::width / 2 - 80.0f, Window::height-20.0f, 0.5f);
            textMesh->draw("FPS: " + std::to_string((int)(1.0f / deltaTime)), 5.0f, Window::height-50.0f, 0.4f);
            textMesh->draw("GameObjects: " + std::to_string(gm->getGameObjectsSize()), 5.0f, Window::height-70.0f, 0.4f);
            textMesh->draw("Particle Systems: " + std::to_string(gm->getVoxelParticlesSize()), 5.0f, Window::height-90.0f, 0.4f);

            textMesh->draw("Camera position: " + std::to_string(camera->getPosition().x)    + " " + std::to_string(camera->getPosition().y)    + " " + std::to_string(camera->getPosition().z), 5.0f,    Window::height - 115.0f, 0.4f);
            textMesh->draw("Player position: " + std::to_string(appleobj->getPosition().x)  + " " + std::to_string(appleobj->getPosition().y)  + " " + std::to_string(appleobj->getPosition().z), 5.0f,  Window::height - 135.0f, 0.4f);
            textMesh->draw("Camera Front: "    + std::to_string(camera->getFrontVector().x) + " " + std::to_string(camera->getFrontVector().y) + " " + std::to_string(camera->getFrontVector().z), 5.0f, Window::height - 155.0f, 0.4f);
        }

        Window::swapBuffers();
        Input::pullEvents();
    }
    std::cout << "\n";
    ResourceManager::cleanup();
    Input::cleanup();
    Window::exit();
    return 0;
}