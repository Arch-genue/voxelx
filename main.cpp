#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
using namespace glm;

#include "window.h"
#include "input.h"
#include "camera.h"

#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/mesh.h"
#include "graphics/vox_renderer.h"
#include "graphics/particles.h"

#include "loaders/resourceloader.h"
#include "loaders/png_loading.h"

#include "voxels/voxel.h"
#include "gameobject.h"
#include "entity.h"

#include <vector>
#include <algorithm>

#define MODELSIZE 1.0f

int WIDTH = 800;
int HEIGHT = 600;

float vertices[] = {
    //x     y
    0.0f,-0.02f,
    0.0f, 0.02f,

    -0.015f, 0.0f,
    0.015f,0.0f,
};

int attrs[2] = {
	2,  0 //null terminator
};

int main() {
    Window::init(WIDTH, HEIGHT, "Voxel3D");
    Input::init();

    //? Loaders
    ResourceLoader::setPath("../res/");
    ResourceLoader::loadShaders();
    ResourceLoader::loadTextures();
    ResourceLoader::loadModels();

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

    _voxels* applevox = load_model("../res/models/apple.voxtxt", "voxtxt");

    _voxels* watervox = new _voxels;
    watervox->m_size = vec3(100, 5, 100);
    std::vector<voxel_m> voxs;
    watervox->voxels = voxs;
    
    size_t i = 0;
    for (size_t y = 0; y < watervox->m_size.y; y++) {
        for (size_t z = 0; z < watervox->m_size.z; z++) {
            for (size_t x = 0; x < watervox->m_size.x; x++) {
                voxel_m vox;
                vox.visible = true;
                vox.position = vec3(x, y, z);
                vox.clr = vec4(0.0f, 0.0f, 1.0f, 0.5f);
                watervox->voxels.push_back(vox);
                i++;
            }
        }
    }

    VoxelRenderer* renderer = new VoxelRenderer(1024*1024*10);

    GameObject* appleobj = new GameObject(renderer, *applevox, voxshader);
    GameObject* apple1obj = new GameObject(renderer, *applevox, voxshader);
    GameObject* waterobj = new GameObject(renderer, *watervox, voxshader);
    
    glClearColor(0.6f, 0.6f, 0.6f,1);
    
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Mesh* crosshair = new Mesh(new _voxels, vertices, 4, attrs);
    Camera* camera = new Camera(vec3(10,1,5), radians(70.0f));

    float lastTime = glfwGetTime();
    float delta = 0.0f;

    float camX = 0.0f;
    float camY = 0.0f;

    float speed = 5;

    bool test = true;

    // std::vector<particle_m> buffer;
    // int bufferSize = 1000;

    // buffer.reserve(bufferSize);
    // for (int i = 0; i < bufferSize; i++) {
    //     particle_m p;
    //     buffer.push_back(p);
    // }

    // создаем объект эффекта
    VoxelParticles effect(1000, renderer, voxshader);

    // добавляем частицы
    for (int i = 0; i < 1000; ++i) {
        particle_m particle;
        particle.position.x = 0.0f;
        particle.position.y = 0.0f;
        particle.position.z = 0.0f;
        particle.velocity.x = rand() / static_cast<float>(RAND_MAX) - 0.5f;
        particle.velocity.y = rand() / static_cast<float>(RAND_MAX) - 0.5f;
        particle.velocity.z = rand() / static_cast<float>(RAND_MAX) - 0.5f;
        particle.lifetime = 5.0f;
        effect.addParticle(particle);
    }

    while (!Window::isShouldClose()) {
        float currentTime = glfwGetTime();
        delta = currentTime - lastTime;
        lastTime = currentTime;

        if (Input::jpressed(GLFW_KEY_ESCAPE)) {
            Window::setShouldClose(true);
        }
        if (Input::jpressed(GLFW_KEY_TAB)) {
            Input::toggleCursor();
        }
        
        if (Input::pressed(GLFW_KEY_W)) {
            camera->position += camera->front * delta * speed;
        }
        if (Input::pressed(GLFW_KEY_S)) {
            camera->position -= camera->front * delta * speed;
        }
        if (Input::pressed(GLFW_KEY_A)) {
            camera->position -= camera->right * delta * speed;
        }
        if (Input::pressed(GLFW_KEY_D)) {
            camera->position += camera->right * delta * speed;
        }

        if (Input::_cursor_locked) {
            camX += -Input::deltaX / Window::height;
            camY += -Input::deltaY / Window::height;
            if (camY < -radians(89.0f)) camY = -radians(89.0f);
            if (camY > radians(89.0f)) camY = radians(89.0f);

            camera->rotation = mat4(1.0f);
            camera->rotate(camY, camX, 0);
        }

        // {
        //     vec3 end;
        //     vec3 norm;
        //     vec3 iend;
        //     Voxel* vox = chunks->rayCast(camera->position, camera->front, 5.0f, end, norm, iend);
        //     if (vox != nullptr) {
        //         if (Input::jclicked(GLFW_MOUSE_BUTTON_1)) {
        //             chunks->set((int)iend.x, (int)iend.y, (int)iend.z, 0);
        //         }
        //         if (Input::jclicked(GLFW_MOUSE_BUTTON_2)) {
        //             chunks->set((int)iend.x+(int)(norm.x), (int)iend.y+(int)(norm.y), (int)iend.z+(int)(norm.z), 2);
        //         }
        //     }
        // }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        voxshader->use();
        voxshader->uniformMatrix("projview", camera->getProjection()*camera->getView());
        
        effect.Update(delta);

        // отрисовываем частицы
        effect.draw();

        glm::vec3 gravity(0.0f, -9.81f, 0.0f);
        apple1obj->applyForce(gravity);
        apple1obj->updatePhysics(delta);
        
        if (test) {
            appleobj->setPosition(vec3(10, 0, 0));
            test = false;
        }

        appleobj->applyForce(gravity*2.0f);
        appleobj->updatePhysics(delta);
        
        apple1obj->draw();
        appleobj->draw();
        //waterobj->draw();

        crosshairShader->use();
        crosshair->draw(GL_LINES);
        
        Window::swapBuffers();
        Input::pullEvents();
    }

    Window::exit();
    delete voxshader;
    delete crosshairShader;

    delete applevox;

    delete appleobj;
    delete apple1obj;
    delete waterobj;

    delete renderer;
    return 0;
}