/*
Voxel3D Engine 2023


C++, OpenGL
*/

#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
// #include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
using namespace glm;

#include "window/newwindow.h"
//#include "window/newinput.h"

#include "graphics/shader.h"
#include "graphics/mesh.h"
#include "graphics/renderer.h"
#include "graphics/particles.h"

#include "loaders/resourceloader.h"

#include "voxels/voxel.h"
#include "gamesystems/camera.h"
#include "gamesystems/gameobject.h"
#include "gamesystems/entity.h"

int WIDTH = 1024;
int HEIGHT = 768;

bool quit;

int main() { // int argc, char *argv[]
    Window::init(WIDTH, HEIGHT, "Voxel3D Alpha");
    //Input::init();

    //TODO ResourceLoader
    // ResourceLoader::setPath("../res/");
    // ResourceLoader::loadShaders();
    // ResourceLoader::loadTextures();
    // ResourceLoader::loadModels();

    Shader* voxshader = load_shader("../res/shaders/voxel.glslv", "../res/shaders/voxel.glslf");
    if (voxshader == nullptr) {
        std::cerr << "Failed to load VOXEL shader\n";
        Window::exit();
        return 1;
    }
    Shader* crosshairShader = load_shader("../res/shaders/crosshair.glslv", "../res/shaders/crosshair.glslf");
    if (crosshairShader == nullptr) {
        std::cerr << "Failed to load crosshair shader\n";
        Window::exit();
        return 1;
    }
    Shader* bboxshader = load_shader("../res/shaders/bbox.glslv", "../res/shaders/bbox.glslf");
    if (bboxshader == nullptr) {
        std::cerr << "Failed to load bbox shader\n";
        Window::exit();
        return 1;
    }

    _voxels* applevox = load_model("../res/models/apple.voxtxt", "voxtxt");

     _voxels* nullvox = new _voxels;
    nullvox->m_size = vec3(1, 1, 1);
    voxel_m vox;
    vox.position = vec3(0, 0, 0);
    vox.clr = vec4(0.5f, 1.0f, 0.5f, 1.0f);
    vox.visible = true;
    nullvox->voxels.push_back(vox);

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
                if (
                    ( x == 0 || x == wallvoxleft->m_size.x-1 ) ||
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

    Renderer* renderer = new Renderer(1024 * 1024 * 10, 100, 100);
    renderer->addShader(voxshader);       // 0
    renderer->addShader(crosshairShader); // 1
    renderer->addShader(bboxshader);      // 2

    renderer->addRowModel("null", nullvox);
    renderer->addRowModel("floor", floorvox);
    renderer->addRowModel("apple", applevox);

    renderer->addRowModel("wallleft", wallvoxleft);
    renderer->addRowModel("wallright", wallvoxright);
    renderer->addRowModel("walltop", wallvoxtop);

    GameObject* wallleftobj = new GameObject(renderer, "wallleft");
    wallleftobj->setCollision(SIMPLE_COLLISION);
    wallleftobj->setPosition(vec3(30, 0, 0));
    GameObject* wallrightobj = new GameObject(renderer, "wallright");
    wallrightobj->setCollision(SIMPLE_COLLISION);
    wallrightobj->setPosition(vec3(-20, 0, 0));
    GameObject* walltopobj = new GameObject(renderer, "walltop");
    walltopobj->setCollision(SIMPLE_COLLISION);
    walltopobj->setPosition(vec3(-20, 50, 0));

    GameObject* floorobj = new GameObject(renderer, "floor");
    floorobj->setCollision(SIMPLE_COLLISION);
    floorobj->setPosition(vec3(-500, -1, -500));

    GameObject* nullobj = new GameObject(renderer, "null");
    nullobj->setPosition(vec3(15,0,0));
    
    GameObject* appleobj = new GameObject(renderer, "apple");
    appleobj->setCollision(SIMPLE_COLLISION);
    appleobj->setPosition(vec3(0, 50, 40));
    appleobj->setHidden(true);

    //? Crosshair
    float vertices[] = {
        //x     y
        0.0f,-0.02f,
        0.0f, 0.02f,

        -0.015f, 0.0f,
        0.015f,0.0f,
    };
    int attrs[2] = { 2,  0 };
    Mesh* crosshair = new Mesh(vertices, 4, attrs);
    Camera* camera = new Camera(vec3(3, 1, 0), radians(70.0f));
    renderer->addCamera(camera);
    appleobj->attachCamera(camera, vec3(0, 15, 0)); //! Attach camera to Apple

    //!TESTING PARTICLES
    //VoxelParticles** effects = new VoxelParticles*[50];
    for (int i = 0; i < 50; i++) {
        //effects[i] = new VoxelParticles(renderer, EFFECT_FLAME, 20);
        //effects[i]->setPosition(vec3(i*10.0f, 20.0f, 30.0f));
    }

    VoxelParticles* effect1 = new VoxelParticles(renderer, EFFECT_FLAME, 20);
    effect1->setPosition(vec3(4.0f, 13.5f, 1.5f));

    //float lastTime = glfwGetTime();
    float lastTime = SDL_GetTicks();
    double deltaTime = 0.0f;
    vec2 cam(0.0f, 0.0f);
    float speed = 50;

    uint64 NOW = SDL_GetPerformanceCounter();
    uint64 LAST = 0;

    Window::_glInit();

    //! ===MAIN LOOP===
    quit = false;
	while (!quit) {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();

        deltaTime = (double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency() );// / 10000;

		while (SDL_PollEvent(&Window::sdlEvent) != 0) {
			if (Window::sdlEvent.type == SDL_QUIT) {
				quit = true;
			}
		}
        Window::_glClear();
        

        renderer->getDefaultShader()->use();
        renderer->getDefaultShader()->uniformMatrix("projview", renderer->getCamera()->getProjection()*renderer->getCamera()->getView());
        
        effect1->draw(deltaTime);

        // walltopobj->draw();
        // wallleftobj->draw();
        // wallrightobj->draw();
        
        floorobj->updatePhysics(deltaTime);
        appleobj->updatePhysics(deltaTime);

        floorobj->draw();
        appleobj->draw();

        crosshairShader->use();
        crosshair->draw(GL_LINES);

	    Window::swapBuffers();
	}
    Window::exit();

    // while (Window::isShouldClose(event)) {
    
    //     // if (Input::jpressed(GLFW_KEY_ESCAPE)) Window::setShouldClose(true);
    //     // if (Input::jpressed(GLFW_KEY_TAB)) Input::toggleCursor();

    //     // // if (Input::pressed(GLFW_KEY_W)) camera->position += camera->front * deltaTime * speed;
    //     // // if (Input::pressed(GLFW_KEY_S)) camera->position -= camera->front * deltaTime * speed;
    //     // // if (Input::pressed(GLFW_KEY_A)) camera->position -= camera->right * deltaTime * speed;
    //     // // if (Input::pressed(GLFW_KEY_D)) camera->position += camera->right * deltaTime * speed;
        
    //     // if (Input::pressed(GLFW_KEY_W)) appleobj->setPosition(appleobj->getPosition() + vec3(camera->front.x, 0, camera->front.z) * deltaTime * speed); //appleobj->translate(-1.0f, vec3(1, 0, 0));
    //     // if (Input::pressed(GLFW_KEY_S)) appleobj->setPosition(appleobj->getPosition() - vec3(camera->front.x, 0, camera->front.z) * deltaTime * speed); //appleobj->translate(1.0f, vec3(1, 0, 0));
    //     // if (Input::pressed(GLFW_KEY_A)) appleobj->setPosition(appleobj->getPosition() - vec3(camera->right.x, 0, camera->right.z) * deltaTime * speed); //appleobj->translate(1.0f, vec3(0, 0, 1));
    //     // if (Input::pressed(GLFW_KEY_D)) appleobj->setPosition(appleobj->getPosition() + vec3(camera->right.x, 0, camera->right.z) * deltaTime * speed); //appleobj->translate(-1.0f, vec3(0, 0, 1));
    //     // if (Input::jpressed(GLFW_KEY_SPACE)) { 
    //         // if (!appleobj->onGround()) 
    //     //     appleobj->applyForce(vec3(0, 10000.0f, 0)); 
    //     // }

    //     // if (Input::_cursor_locked) {
    //     //     cam.x += -Input::deltaX / Window::height; 
    //     //     cam.y += -Input::deltaY / Window::height;
    //     //     if (cam.y < -radians(89.0f)) cam.y = -radians(89.0f);
    //     //     if (cam.y > radians(89.0f)) cam.y = radians(89.0f);

    //     //     camera->rotation = mat4(1.0f);
    //     //     camera->rotate(cam, 0);
    //     // }

    //     Window::_glClear();
    //     renderer->getDefaultShader()->use();
	//     renderer->getDefaultShader()->uniformMatrix("projview", renderer->getCamera()->getProjection()*renderer->getCamera()->getView());
        
    //     effect1->draw(deltaTime);
    //     voxshader->uniformMatrix("projview", camera->getProjection()*camera->getView());
        
    //     walltopobj->draw();
    //     wallleftobj->draw();
    //     wallrightobj->draw();
        
    //     floorobj->updatePhysics(deltaTime);
    //     appleobj->updatePhysics(deltaTime);

    //     floorobj->draw();
    //     appleobj->draw();

    //     if (appleobj->CheckCollision(floorobj->getBBOX()).y == 0) {
    //         appleobj->applyForce(vec3(0, -9.8, 0));
    //         test = false;
    //     } else {
    //         appleobj->setVelocity(glm::vec3(0, 0, 0));
    //         appleobj->setAcceleration(glm::vec3(0, 0, 0));
    //         if (!test) {
    //             appleobj->setPosition(vec3(appleobj->getPosition().x, floorobj->getBBOX().max.y, appleobj->getPosition().z));
    //             test = true;
    //         }
            
    //     }

    //     // if (Input::jclicked(GLFW_MOUSE_BUTTON_2)) {
    //     //     //floorobj->translate(-15.0f, vec3(0, 1, 0));
    //     //     appleobj->setPosition(vec3(0, 100, 0));
    //     // }
    //     // if (Input::jclicked(GLFW_MOUSE_BUTTON_1)) {
    //     //     appleobj->translate(1.0f, vec3(0, 1, 0));

    //     //     vec3 end; vec3 norm; vec3 iend;
    //     //     if (appleobj->raycast(camera->getPosition(), camera->front, 10.0f, end, norm, iend) ) {
    //     //         appleobj->setVisible(false);
    //     //     }
    //     // }
    //     // std::cout << camera->getPosition().x << " " << camera->getPosition().y   << " " << camera->getPosition().z << " " << std::endl;
    //     // nullobj->draw();

    //     crosshairShader->use();
    //     crosshair->draw(GL_LINES);    
    //     Window::swapBuffers();
    //     //Input::pullEvents();
    // }

    // Window::exit();
    // // delete voxshader;
    // // delete crosshairShader;

    // // delete nullvox;
    // // delete wallvox;
    // // delete applevox;

    // delete effect1;

    // delete nullobj;
    // delete appleobj;

    // delete renderer;

    return 0;
}