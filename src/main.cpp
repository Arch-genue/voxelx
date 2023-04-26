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
#include "graphics/voxelparticles.h"
#include "graphics/gui.h"

#include "loaders/resourcemanager.h"
#include "gamesystems/gamemanager.h"

#include "voxels/voxel.h"
#include "gamesystems/camera.h"
#include "gamesystems/gameobject.h"
#include "gamesystems/entity.h"

int WIDTH = 1024;
int HEIGHT = 768;
float MOUSE_SPEED = 1.3f;

int main() {
    Window::init(WIDTH, HEIGHT, "Voxel3D Alpha");
    Input::init();
    ResourceManager::init("../res/");

    GUI gui;

    //! Global renderer
    Renderer::init(1024*1024);

    ResourceManager::loadShader("voxel");
    ResourceManager::loadShader("particle");
    ResourceManager::loadShader("crosshair");
    ResourceManager::loadShader("ui");

    ResourceManager::loadTexture("slot");

    ResourceManager::loadModel("apple", "voxtxt"); //? voxtxt, generic, null, voxlap
    ResourceManager::loadModel("null", "null");

    GameManager gm(100);

    _voxels* floorvox = new _voxels;
    for(uint16_t i = 0; i < 6; i++) floorvox->light[i] = glm::vec3(1);
    floorvox->m_size = vec3(1000, 1, 1000);
    floorvox->renderSide = "top";
    for (size_t y = 0; y < floorvox->m_size.y; y++) {
        for (size_t z = 0; z < floorvox->m_size.z; z++) {
            for (size_t x = 0; x < floorvox->m_size.x; x++) {
                voxel_m vox;
                vox.position = vec3(x, y, z);
                vox.clr = vec4(0.3f, 0.3f, 0.3f, 1.0f);
                vox.size = 1.0f;
                vox.visible = true;
                floorvox->voxels.push_back(vox);
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
    appleobj->setHidden(true);

    GameObject* appleobj1 = new GameObject("apple");
    appleobj1->setPosition(vec3(0, 10, 0));
    appleobj1->setRigidBody(true);
    appleobj1->setCollision(SIMPLE_COLLISION);

    gm.addGameObject(floorobj);
    gm.addGameObject(appleobj);
    gm.addGameObject(appleobj1);

    //! Camera
    Camera* camera = new Camera(vec3(3, 1, 0), radians(70.0f)); 
    Renderer::addCamera(camera);
    appleobj->attachCamera(camera, vec3(0, 15, 0)); //! Attach camera to appleobj, apple hidden

    //! Particles
    VoxelParticles* effect1 = new VoxelParticles(EFFECT_FLAME, 20);
    effect1->setPosition(vec3(4.0f, 30.0f, 1.5f));

    VoxelParticles* effect2 = new VoxelParticles(EFFECT_CURSED_FLAME, 30);
    effect2->setPosition(vec3(15.0f, 30.0f, 1.5f));

    VoxelParticles* effect3 = new VoxelParticles(EFFECT_DEAD_FLAME, 30);
    effect3->setPosition(vec3(-10.0f, 30.0f, 1.5f));

    // gm.addEffect();
    // gm.addEffect();
    // gm.addEffect();

    float deltaTime = 0.0f;
    auto lastTimePoint = std::chrono::system_clock::now();
    vec2 cam(0.0f, 0.0f);
    float speed = 50;
    
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

            camera->rotation = mat4(1.0f);
            camera->rotate(cam, 0);
        }
        

        if (Input::jpressed(SDLK_ESCAPE)) quit = true;
        if (Input::jpressed(SDLK_p)) {
            Window::setPause(!Window::getPause());
            Input::toggleCursor();
        }
        if (Input::jpressed(SDLK_TAB)) Input::toggleCursor();

        if (Input::pressed(SDLK_w)) appleobj->setPosition( appleobj->getPosition() + vec3(camera->front.x, 0, camera->front.z) * deltaTime * speed );
        if (Input::pressed(SDLK_s)) appleobj->setPosition( appleobj->getPosition() - vec3(camera->front.x, 0, camera->front.z) * deltaTime * speed );
        if (Input::pressed(SDLK_a)) appleobj->setPosition( appleobj->getPosition() - vec3(camera->right.x, 0, camera->right.z) * deltaTime * speed );
        if (Input::pressed(SDLK_d)) appleobj->setPosition( appleobj->getPosition() + vec3(camera->right.x, 0, camera->right.z) * deltaTime * speed );
        
        //if (appleobj->checkGround()) {
        if (Input::jpressed(SDLK_SPACE)) appleobj->getPhysics()->applyForce(vec3(0, 300.0f, 0));
        //}

        if (Input::pressed(SDLK_c)) camera->zoom = 0.5f; else camera->zoom = 1.0f;
        if (Input::jclicked(SDL_BUTTON_RIGHT)) appleobj->setPosition(vec3(0, 100, 0));
        if (Input::jclicked(SDL_BUTTON_LEFT)) {
            appleobj->translate(1.0f, vec3(0, 1, 0));
            vec3 end; vec3 norm; vec3 iend;
            if (appleobj->raycast(camera->getPosition(), camera->front, 10.0f, end, norm, iend) ) appleobj->setVisible(false);
        }

        Window::_glClear();
        
        ResourceManager::getShader("voxel")->use();
        ResourceManager::getShader("voxel")->uniformMatrix("projview", Renderer::getCamera()->getProjection() * Renderer::getCamera()->getView());

        gm.UpdatePhysics(deltaTime);
        gm.Update();

        ResourceManager::getShader("particle")->use();
        ResourceManager::getShader("particle")->uniformMatrix("projviewmodel", Renderer::getCamera()->getProjection() * Renderer::getCamera()->getView() * glm::scale(glm::mat4(1.0f), glm::vec3(0.05f)));
        
        // gm.UpdateEffect();
        effect1->draw(deltaTime);
        effect2->draw(deltaTime);
        effect3->draw(deltaTime);

        camera->setPosition( (appleobj->getPosition() + vec3(0, 15, 0)) / 10.0f );

        gui.draw();
        
	    Window::swapBuffers();
        Input::pullEvents();
	}
    Window::exit();

    delete effect1;
    delete effect2;
    return 0;
}