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