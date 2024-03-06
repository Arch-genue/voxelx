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
#include <random>

#define GLEW_STATIC
#include <GL/glew.h>
// #include <SDL2/SDL.h>
// #include <SDL2/SDL_opengl.h>
// #include <SDL2/SDL_ttf.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
using namespace glm;

// #include <boost/python.hpp>

#include "window/window.h"
#include "window/input.h"

#include "graphics/renderer.h"
#include "graphics/gui.h"
#include "graphics/textmesh.h"

#include "loaders/resourcemanager.h"
#include "gamesystems/gamemanager.h"

#include "voxels/voxelmodel.h"
// #include "voxels/particlesmodel.h"
#include "gamesystems/camera.h"
#include "gamesystems/entity.h"

#include "system/taskscheduler.h"

#include "graphics/types/octotree.h"

/**
 * @brief Версия Voxel3D Engine
 */
#define _VERSION "0.2.3"

/**
 * @brief Размера буфера рендера
 *
 */
#define _RENDERSIZE 4096 * 4096

/**
 * @brief Заголовок окна
 *
 */
#define TITLE "VoxelX 0.2"

int WIDTH = 1024;
int HEIGHT = 768;

bool DEBUG_MODE = true;
bool QUIT = false;

float MOUSE_SPEED = 1.3f;

void exit_game() {
    QUIT = true;
}

void debug_mode() {
    DEBUG_MODE = !DEBUG_MODE;
}

void pause_mode() {
    Window::setPause(!Window::getPause());
    Input::toggleCursor();
}

int test() {
    return 5;
}

// using namespace boost::python;

// BOOST_PYTHON_MODULE( example ) {
//     class_<Renderer>( "Renderer" )
//         // .def( init<int>( args( "some_id" ) ) )
//         .def( "INIT", &Renderer::init )
//         // .def_static()
//     ;
// }

// Определение модуля Python с использованием Boost.Python
// BOOST_PYTHON_MODULE(libvoxel3d) {
//     using namespace boost::python;
    
//     // Экспорт функции add в модуль Python
//     def("add", debug_mode);
//     def("test", test);

//     class_<ResourceManager>("ResourceManager") // Конструктор с одним аргументом типа int
//         .def("init", &ResourceManager::init, args( "name" ))
//         .def("loadShaders", &ResourceManager::loadShaders)
//     ;
// }

std::mt19937 rng1(std::random_device{}());
int main() {
    Logger::setLogLevel(LOGLEVEL::WARNING);

    std::string _version = _VERSION;
    Logger::eprint("ENGINE", "VoxelX " + _version + "\n", LOGLEVEL::ALL);    

    Window::init(WIDTH, HEIGHT, TITLE);
    Input::init();

    ResourceManager::init("../res/");

    Renderer::init(_RENDERSIZE);

    ResourceManager::loadShaders();
    ResourceManager::loadTextures();
    ResourceManager::loadModels();
    ResourceManager::loadParticlesSystems();
    ResourceManager::loadFonts();

    GameManager *gm = new GameManager();

    //! Camera
    Camera *camera = new Camera(vec3(3, 1, 0), radians(70.0f));
    Renderer::addCamera(camera);

    std::uniform_real_distribution<float> redclr_generator = std::uniform_real_distribution<float>(0.0f, 1.0f);
    std::uniform_real_distribution<float> greenclr_generator = std::uniform_real_distribution<float>(0.0f, 1.0f);
    std::uniform_real_distribution<float> blueclr_generator = std::uniform_real_distribution<float>(0.0f, 1.0f);
    
    VoxelModel *floorvox = new VoxelModel(glm::ivec3(1000, 1, 1000), 10);
    for (int y = 0; y < floorvox->getSize().y; y++) {
        for (int z = 0; z < floorvox->getSize().z; z++) {
            for (int x = 0; x < floorvox->getSize().x; x++) {
                Voxel *voxel = new Voxel(glm::vec3(x, y, z));
                float clr = redclr_generator(rng1);
                voxel->setColor(vec4(clr, clr, clr, 1.0f));
                floorvox->addVoxel(voxel);
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
    ResourceManager::addModel(boxvox, "box");
    ResourceManager::addModel(lightvox, "light");

    ResourceManager::prepareModel("floor");
    ResourceManager::prepareModel("box");
    ResourceManager::prepareModel("light");

    glm::vec3 min(-100.0f, 0.0f, -100.0f);
    glm::vec3 max(200.0f);
    OctoTree* tree = new OctoTree(min, max);
    tree->setMaxDepth(4);

    // ! GameObjects
    GameObject *floorobj = new GameObject("floor", vec3(-50, -0.1f, -50));
    floorobj->setPhysics(STATIC_PHYSICS);

    GameObject *boxobj = new GameObject("box", vec3(0, 0, 0));
    boxobj->setPhysics(DYNAMIC_PHYSICS);

    GameObject *lightobj = new GameObject("light", vec3(0, 20, -50));
    lightobj->setPhysics(STATIC_PHYSICS);

    GameObject *appleobj = new GameObject("apple", vec3(0, 50, 40));
    appleobj->attachCamera(camera, vec3(0, 50, 40)); //! Attach camera to appleobj, apple hidden
    appleobj->setPhysics(DYNAMIC_PHYSICS);
    appleobj->setVisible(true);

    // auto test1 = [tree](GameObject* gameobject) {
    //     return tree->getRoot()->insert(gameobject);
    // };
    // float time = measureFunctionTime(test1, appleobj);
    // std::cout << time << std::endl;
    tree->generate();

    Logger::eprint("OCTREE", "Node count: " + std::to_string(tree->getRoot()->getNodeCount()), LOGLEVEL::INFO);

    GameObject *appleobj1 = new GameObject("apple", vec3(0, 15, 10));
    appleobj1->setPhysics(DYNAMIC_PHYSICS);

    // GameObject *appleobj2 = new GameObject("apple", vec3(20, 30, 20));
    // appleobj2->setPhysics(DYNAMIC_PHYSICS);

    // GameObject *breadobj = new GameObject("bread", vec3(50, 30, 20));
    // breadobj->setPhysics(DYNAMIC_PHYSICS);

    gm->addGameObject(floorobj);
    gm->addGameObject(boxobj);
    gm->addGameObject(lightobj);
    gm->addGameObject(appleobj);
    gm->addGameObject(appleobj1);
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
    float speed = 10.0f;
    float jumpforce = 20.0f;

    GUI gui;

    FT_Face face = ResourceManager::getFont("arial");
    TextMesh* textMesh = new TextMesh(face);

    Input::add_event_handler(JPRESSED, SDLK_ESCAPE, exit_game);
    Input::add_event_handler(JPRESSED, SDLK_TAB, Input::toggleCursor);
    Input::add_event_handler(JPRESSED, SDLK_F3, debug_mode);
    Input::add_event_handler(JPRESSED, SDLK_F11, Window::toggleFullscreen);
    Input::add_event_handler(JPRESSED, SDLK_p, pause_mode);

    Logger::eprint("DBG", "VEC3 Size " + std::to_string(sizeof(glm::vec3(1.0f))), LOGLEVEL::WARNING);
    Logger::eprint("DBG", "IVEC3 Size " + std::to_string(sizeof(glm::ivec3(1))), LOGLEVEL::WARNING);
    Logger::eprint("DBG", "Voxel Size " + std::to_string(sizeof(Voxel)), LOGLEVEL::WARNING);
    Logger::eprint("DBG", "VoxelModel Size " + std::to_string(sizeof(VoxelModel)), LOGLEVEL::WARNING);
    Logger::eprint("DBG", "Array3 Size " + std::to_string(sizeof(vtype::array3<int, Voxel*>)), LOGLEVEL::WARNING);
    Logger::eprint("DBG", "Mesh Size " + std::to_string(sizeof(Mesh)), LOGLEVEL::WARNING);

    Window::setPause(false);
    Input::toggleCursor();
    float angle = 0.0f;
    Window::sky.x = 0.0f;
    Window::sky.y = 0.0f;
    Window::sky.z = 0.0f;
    while (!QUIT) {
        Input::processEvents(QUIT);
        Input::process_keys();

        auto newTimePoint = std::chrono::system_clock::now();
        auto dtMsec = std::chrono::duration_cast<std::chrono::milliseconds>(newTimePoint - lastTimePoint);
        lastTimePoint = newTimePoint;
        deltaTime = 0.001f * float(dtMsec.count());

        Window::_glClear();

        if (Input::getCursorLock()) {
            cam.x += -Input::deltaX * MOUSE_SPEED / Window::height;
            cam.y += -Input::deltaY * MOUSE_SPEED / Window::height;
            if (cam.y < -glm::radians(89.0f))
                cam.y = -glm::radians(89.0f);
            else if (cam.y > glm::radians(89.0f))
                cam.y = glm::radians(89.0f);

            camera->setRotation(glm::mat4(1.0f));
            camera->rotate(cam, 0);
            appleobj->setRotation(cam.x, glm::vec3(0, 1, 0));
            camera->setPosition(appleobj->getPosition() + glm::vec3(0, 3, 0));
        }
        glm::vec3 camera_front = camera->getTarget();
        glm::vec3 camera_right = camera->getRight();

        if (!Window::getPause()) {
            if (Input::pressed(SDLK_w))
                appleobj->setPosition(appleobj->getPosition() + glm::vec3(camera_front.x, 0, camera_front.z) * deltaTime * speed);
            if (Input::pressed(SDLK_s))
                appleobj->setPosition(appleobj->getPosition() - glm::vec3(camera_front.x, 0, camera_front.z) * deltaTime * speed);
            if (Input::pressed(SDLK_a))
                appleobj->setPosition(appleobj->getPosition() - glm::vec3(camera_right.x, 0, camera_right.z) * deltaTime * speed);
            if (Input::pressed(SDLK_d))
                appleobj->setPosition(appleobj->getPosition() + glm::vec3(camera_right.x, 0, camera_right.z) * deltaTime * speed);

            if (Input::pressed(SDLK_u)) {
                lightobj->setPosition(lightobj->getPosition() + glm::vec3(0, 0, -1) * speed/10.0f);
                boxobj->setPosition(boxobj->getPosition() + glm::vec3(0, 0, -1) * speed/10.0f);
            } else if (Input::pressed(SDLK_h)) {
                lightobj->setPosition(lightobj->getPosition() + glm::vec3(-1, 0, 0) * speed/10.0f);
                boxobj->setPosition(boxobj->getPosition() + glm::vec3(-1, 0, 0) * speed/10.0f);
            } else if (Input::pressed(SDLK_k)) {
                lightobj->setPosition(lightobj->getPosition() + glm::vec3(1, 0, 0) * speed/10.0f);
                boxobj->setPosition(boxobj->getPosition() + glm::vec3(1, 0, 0) * speed/10.0f);
            } else if (Input::pressed(SDLK_j)) {
                lightobj->setPosition(lightobj->getPosition() + glm::vec3(0, 0, 1) * speed/10.0f);
                boxobj->setPosition(boxobj->getPosition() + glm::vec3(0, 0, 1) * speed/10.0f);
            }

            if (appleobj->getPhysicsObject()->isGrounded()) {
                if (Input::jpressed(SDLK_SPACE)) {
                    appleobj->getPhysicsObject()->applyForce(glm::vec3(0, jumpforce * 5, 0));
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
                appleobj->setPosition(glm::vec3(0, 100, 0));
            }
 
            angle += 0.01f;
            float radius = 30.0f;

            lightobj->setPosition(glm::vec3(radius * cos(angle), 10, radius * sin(angle)));
        }

        // ResourceManager::getShader("particle")->use();
        // ResourceManager::getShader("particle")->uniformMatrix("projviewmodel", Renderer::getCamera()->getProjection() * Renderer::getCamera()->getView() * glm::scale(glm::mat4(1.0f), glm::vec3(0.05f)));
        // gm->UpdateParticles(deltaTime);

        Light light;
        light.position = lightobj->getPosition();
        light.direction = camera->getTarget();
        light.cutOff = glm::cos(glm::radians(15.0f));
        light.outerCutOff = glm::cos(glm::radians(30.0f));
        
        light.ambient = glm::vec3(0.1f);
        light.diffuse = glm::vec3(0.8f);
        light.specular = glm::vec3(1.0f);
        light.constant = 0.1f;
        light.linear = 0.014f;
        light.quadratic = 0.00007f;

        ResourceManager::getShader("voxel")->use();        
        ResourceManager::getShader("voxel")->uniformMatrix("projection", Renderer::getCamera()->getProjection());
        ResourceManager::getShader("voxel")->uniformMatrix("view", Renderer::getCamera()->getView());

        gm->UpdatePhysics(deltaTime);
        gm->Update(light);
        
        // gui.draw();

        //? TEXT
        ResourceManager::getShader("font")->use();
        ResourceManager::getShader("font")->uniformMatrix("projection", glm::ortho(0.0f, (float)Window::width, 0.0f, (float)Window::height));
        ResourceManager::getShader("font")->uniformVec3("textColor", glm::vec3(0.9f, 0.3f, 0.9f));
        textMesh->draw("VoxelX: " + _version, 5.0f, (float)Window::height-20.0f, 0.4f);

        if (DEBUG_MODE) {
            ResourceManager::getShader("font")->uniformVec3("textColor", glm::vec3(0.1f, 0.5f, 0.1f));

            textMesh->draw("Debug mode", Window::width / 2 - 80.0f, Window::height-20.0f, 0.5f);
            textMesh->draw("FPS: " + std::to_string((int)(1.0f / deltaTime)), 5.0f, Window::height-50.0f, 0.4f);
            textMesh->draw("GameObjects: " + std::to_string(gm->getGameObjectsSize()), 5.0f, Window::height-70.0f, 0.4f);
            textMesh->draw("Particle Systems: " + std::to_string(gm->getVoxelParticlesSize()), 5.0f, Window::height-90.0f, 0.4f);

            textMesh->draw("Camera position: " + std::to_string(camera->getPosition().x)    + " " + std::to_string(camera->getPosition().y)    + " " + std::to_string(camera->getPosition().z), 5.0f,    Window::height - 115.0f, 0.4f);
            textMesh->draw("Player position: " + std::to_string(appleobj->getPosition().x)  + " " + std::to_string(appleobj->getPosition().y)  + " " + std::to_string(appleobj->getPosition().z), 5.0f,  Window::height - 135.0f, 0.4f);
            textMesh->draw("Camera Front: "    + std::to_string(camera->getTarget().x) + " " + std::to_string(camera->getTarget().y) + " " + std::to_string(camera->getTarget().z), 5.0f, Window::height - 155.0f, 0.4f);
            textMesh->draw("LIGHT Object position: " + std::to_string(lightobj->getPosition().x)  + " " + std::to_string(lightobj->getPosition().y)  + " " + std::to_string(lightobj->getPosition().z), 5.0f,  Window::height - 175.0f, 0.4f);
            textMesh->draw("BOX Object position: " + std::to_string(boxobj->getPosition().x)  + " " + std::to_string(boxobj->getPosition().y)  + " " + std::to_string(boxobj->getPosition().z), 5.0f,  Window::height - 195.0f, 0.4f);
        
            // ResourceManager::getShader("octoline")->use();
            // ResourceManager::getShader("octoline")->uniformMatrix("projviewmodel", Renderer::getCamera()->getProjection() * Renderer::getCamera()->getView() * glm::mat4(1.0f));
            // tree->draw();
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