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

#include "graphics/shader.h"
//#include "graphics/texture.h"
#include "graphics/mesh.h"
#include "graphics/renderer.h"
#include "graphics/particles.h"
#include "graphics/gui.h"

#include "loaders/resourcemanager.h"
#include "gamesystems/gamemanager.h"
//#include "loaders/png_loading.h"

#include "voxels/voxel.h"
#include "gamesystems/camera.h"
#include "gamesystems/gameobject.h"
#include "gamesystems/entity.h"

int WIDTH = 1024;
int HEIGHT = 768;
float MOUSE_SPEED = 1.3f;

bool quit;

int main() {
    Window::init(WIDTH, HEIGHT, "Voxel3D Alpha");
    Input::init();
    ResourceManager::init("../res/");
    GUI gui;

    //! Global renderer
    Renderer::init(1024 * 1024 * 5);

    //! ResourceLoader
    ResourceManager::loadShader("voxel");
    ResourceManager::loadShader("crosshair");
    ResourceManager::loadShader("ui");

    ResourceManager::loadTexture("slot");

    ResourceManager::loadModel("apple", "voxtxt"); //? voxtxt, generic, null, voxlap
    ResourceManager::loadModel("null", "null");

    GameManager gm(100);

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> clr_generator(0.1f, 0.9f);
    _voxels* wallvoxleft = new _voxels;
    wallvoxleft->m_size = vec3(1, 50, 70);
    wallvoxleft->renderSide = "right";
    float clr;
    for (size_t y = 0; y < wallvoxleft->m_size.y; y++) {
        for (size_t z = 0; z < wallvoxleft->m_size.z; z++) {
            for (size_t x = 0; x < wallvoxleft->m_size.x; x++) {
                voxel_m vox;
                clr = clr_generator(rng);
                vox.position = vec3(x, y, z);
                vox.clr = vec4(clr, clr, clr, 1.0f);
                vox.visible = false;
                if (( x == 0 || x == wallvoxleft->m_size.x-1 ) ||
                    ( y == 0 || y == wallvoxleft->m_size.y-1 ) ||
                    ( z == 0 || z == wallvoxleft->m_size.z-1 )
                ) vox.visible = true;
                wallvoxleft->voxels.push_back(vox);
            }
        }
    }

    _voxels* wallvoxright = new _voxels;
    wallvoxright->m_size = vec3(1, 50, 70);
    wallvoxright->renderSide = "left";
    wallvoxright->voxels = wallvoxleft->voxels;

    _voxels* wallvoxtop = new _voxels;
    wallvoxtop->m_size = vec3(50, 1, 70);
    wallvoxtop->renderSide = "bottom";    
    for (size_t y = 0; y < wallvoxtop->m_size.y; y++) {
        for (size_t z = 0; z < wallvoxtop->m_size.z; z++) {
            for (size_t x = 0; x < wallvoxtop->m_size.x; x++) {
                voxel_m vox;
                clr = clr_generator(rng);
                vox.position = vec3(x, y, z);
                vox.clr = vec4(clr, clr, clr, 1.0f);
                vox.visible = false;
                if (
                    ( x == 0 || x == wallvoxtop->m_size.x-1 ) ||
                    ( y == 0 || y == wallvoxtop->m_size.y-1 ) ||
                    ( z == 0 || z == wallvoxtop->m_size.z-1 )
                ) vox.visible = true;
                wallvoxtop->voxels.push_back(vox);
            }
        }
    }

    _voxels* floorvox = new _voxels;
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
    ResourceManager::addModel(wallvoxleft, "wallleft");
    ResourceManager::addModel(wallvoxright, "wallright");
    ResourceManager::addModel(wallvoxtop, "walltop");

    //! GameObjects
    GameObject* wallleftobj = new GameObject("wallleft");
    wallleftobj->setCollision(SIMPLE_COLLISION);
    wallleftobj->setPosition(vec3(30, 0, 0));

    GameObject* wallrightobj = new GameObject("wallright");
    wallrightobj->setCollision(SIMPLE_COLLISION);
    wallrightobj->setPosition(vec3(-20, 0, 0));

    GameObject* walltopobj = new GameObject("walltop");
    walltopobj->setCollision(SIMPLE_COLLISION);
    walltopobj->setPosition(vec3(-20, 50, 0));

    GameObject* floorobj = new GameObject("floor");
    floorobj->setCollision(SIMPLE_COLLISION);
    floorobj->setPosition(vec3(-500, -1, -500));

    GameObject* nullobj = new GameObject("null");
    nullobj->setPosition(vec3(15,0,0));
    
    GameObject* appleobj = new GameObject("apple");
    appleobj->setRigidBody(true);
    appleobj->setCollision(SIMPLE_COLLISION);
    appleobj->setPosition(vec3(0, 50, 40));
    appleobj->setHidden(true);

    // gm.addGameObject(wallleftobj);
    // gm.addGameObject(wallrightobj);
    // gm.addGameObject(walltopobj);
    gm.addGameObject(floorobj);
    gm.addGameObject(nullobj);
    gm.addGameObject(appleobj);

    //! Camera
    Camera* camera = new Camera(vec3(3, 1, 0), radians(70.0f)); 
    camera->perspective = true;
    camera->flipped = false;
    camera->zoom = 1.0f;
    Renderer::addCamera(camera);
    appleobj->attachCamera(camera, vec3(0, 15, 0)); //! Attach camera to appleobj, apple hidden

    //! Particles
    VoxelParticles* effect1 = new VoxelParticles(EFFECT_FLAME, 20);
    effect1->setPosition(vec3(4.0f, 30.0f, 1.5f));

    VoxelParticles* effect2 = new VoxelParticles(EFFECT_CURSED_FLAME, 30);
    effect2->setPosition(vec3(15.0f, 30.0f, 1.5f));

    float deltaTime = 0.0f;
    auto lastTimePoint = std::chrono::system_clock::now();
    vec2 cam(0.0f, 0.0f);
    float speed = 50;
    bool test = false;
    
    //! ===MAIN-LOOP===
    quit = false;
    Window::setPause(false);
    // Input::toggleCursor();
	while (!quit) {
        auto newTimePoint = std::chrono::system_clock::now();
        auto dtMsec = std::chrono::duration_cast<std::chrono::milliseconds>(newTimePoint - lastTimePoint);
        lastTimePoint = newTimePoint;
        deltaTime = 0.001f * float(dtMsec.count());
        
		while (SDL_PollEvent(&Window::sdlEvent) != 0) {
            Uint8 b = Window::sdlEvent.button.button;
            int x1;int y1; SDL_GetMouseState(&x1, &y1);
			if (Window::sdlEvent.type == SDL_QUIT) quit = true;
            else if (Window::sdlEvent.type == SDL_WINDOWEVENT) {
                if (Window::sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED ) Input::window_size_callback(Window::sdlEvent.window.data1, Window::sdlEvent.window.data2);
            }

            else if (Window::sdlEvent.type == SDL_MOUSEMOTION) Input::cursor_position_callback(Window::sdlEvent.motion);
            else if (Window::sdlEvent.type == SDL_MOUSEBUTTONDOWN) Input::mouse_button_callback(b, 1);
            else if (Window::sdlEvent.type == SDL_MOUSEBUTTONUP) Input::mouse_button_callback(b, 0);
            else if (Window::sdlEvent.type == SDL_KEYDOWN) Input::key_callback(Window::sdlEvent.key.keysym.sym, 1);
            else if (Window::sdlEvent.type == SDL_KEYUP) Input::key_callback(Window::sdlEvent.key.keysym.sym, 0);
		}

        if (Input::_cursor_locked) {
            cam.x += -Input::deltaX * MOUSE_SPEED / Window::height; cam.y += -Input::deltaY * MOUSE_SPEED / Window::height;
            if (cam.y < -radians(89.0f)) cam.y = -radians(89.0f);
            if (cam.y > radians(89.0f)) cam.y = radians(89.0f);

            camera->rotation = mat4(1.0f);
            camera->rotate(cam, 0);
        }

        if (Input::jpressed(SDLK_ESCAPE)) {
            quit = true; 
        }
        if (Input::jpressed(SDLK_p)) {
            Window::setPause(!Window::getPause());
            Input::toggleCursor();
        }
        if (Input::jpressed(SDLK_TAB)) Input::toggleCursor();
        
        if (Input::pressed(SDLK_w)) appleobj->setPosition(appleobj->getPosition() + vec3(camera->front.x, 0, camera->front.z) * deltaTime * speed); 
        if (Input::pressed(SDLK_s)) appleobj->setPosition(appleobj->getPosition() - vec3(camera->front.x, 0, camera->front.z) * deltaTime * speed);
        if (Input::pressed(SDLK_a)) appleobj->setPosition(appleobj->getPosition() - vec3(camera->right.x, 0, camera->right.z) * deltaTime * speed);
        if (Input::pressed(SDLK_d)) appleobj->setPosition(appleobj->getPosition() + vec3(camera->right.x, 0, camera->right.z) * deltaTime * speed);
        
        if (appleobj->checkGround()) { 
            if (Input::jpressed(SDLK_SPACE)) {
                appleobj->applyForce(vec3(0, 5000.0f, 0));
            }
             
        }

        if (Input::pressed(SDLK_c)) camera->zoom = 0.5f; else camera->zoom = 1.0f;

        if (Input::jclicked(SDL_BUTTON_RIGHT)) {
            appleobj->setPosition(vec3(0, 100, 0));
        }
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

        effect1->draw(deltaTime);
        effect2->draw(deltaTime);

        camera->setPosition( (appleobj->getPosition() + vec3(0, 15, 0)) / 10.0f );

        gui.draw();
        
	    Window::swapBuffers();
        Input::pullEvents();
	}
    Window::exit();

    delete effect1;
    return 0;
}