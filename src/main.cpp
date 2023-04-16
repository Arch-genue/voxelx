#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
using namespace glm;

#include "window/window.h"
#include "window/input.h"
#include "gamesystems/camera.h"

#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/mesh.h"
#include "graphics/renderer.h"
#include "graphics/particles.h"

#include "loaders/resourceloader.h"
#include "loaders/png_loading.h"

#include "voxels/voxel.h"
#include "gamesystems/gameobject.h"
#include "gamesystems/entity.h"

// #include <freetype2/ft2build.h>
// #include FT_FREETYPE_H
// #include <map>

#define MODELSIZE 1.0f

int WIDTH = 800;
int HEIGHT = 600;

//Crosshair
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

    //Shader* fontshader = load_shader("../res/shaders/font.glslv", "../res/shaders/font.glslf");

    Shader* crosshairShader = load_shader("../res/shaders/crosshair.glslv", "../res/shaders/crosshair.glslf");
    if (crosshairShader == nullptr) {
        std::cerr << "Failed to load crosshair shader\n";
        Window::exit();
        return 1;
    }

    _voxels* applevox = load_model("../res/models/apple.voxtxt", "voxtxt");

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> clr_generator(0.1f, 0.9f);
    _voxels* wallvox = new _voxels;
    wallvox->m_size = vec3(30, 50, 30);
    float clr;
    for (size_t y = 0; y < wallvox->m_size.y; y++) {
        for (size_t z = 0; z < wallvox->m_size.z; z++) {
            for (size_t x = 0; x < wallvox->m_size.x; x++) {
                voxel_m vox;
                clr = clr_generator(rng);
                vox.position = vec3(x, y, z);
                vox.clr = vec4(clr, clr, clr, 1.0f);
                vox.visible = false;
                if (
                    ( x == 0 || x == wallvox->m_size.x-1 ) ||
                    ( y == 0 || y == wallvox->m_size.y-1 ) ||
                    ( z == 0 || z == wallvox->m_size.z-1 )
                ) vox.visible = true;
                wallvox->voxels.push_back(vox);
            }
        }
    }
    //GameObject** objs = new GameObject*[100];

    _voxels* nullvox = new _voxels;
    nullvox->m_size = vec3(1, 1, 1);
    voxel_m vox;
    vox.position = vec3(0, 0, 0);
    vox.clr = vec4(0.5f, 1.0f, 0.5f, 1.0f);
    vox.visible = true;
    nullvox->voxels.push_back(vox);

    Renderer* renderer = new Renderer(1024*1024*10, 100, 100);
    renderer->addShader(voxshader); // 0
    renderer->addShader(crosshairShader); // 1

    GameObject* nullobj = new GameObject(renderer, *nullvox, voxshader);
    nullobj->setPosition(vec3(15,0,0));

    GameObject* appleobj = new GameObject(renderer, *applevox, voxshader);

    Window::glInit();

    Mesh* crosshair = new Mesh(new _voxels, vertices, 4, attrs);
    Camera* camera = new Camera(vec3(3, 0, 0), radians(70.0f));

    float lastTime = glfwGetTime();
    float deltaTime = 0.0f;

    float camX = 0.0f;
    float camY = 0.0f;

    float speed = 5;

    //!TESTING PARTICLES
    VoxelParticles* effect1 = new VoxelParticles(20, renderer, voxshader);
    effect1->setType(EFFECT_FLAME);
    effect1->setPosition(vec3(2.0f, 5.0f, 2.0f));

    while (!Window::isShouldClose()) {
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        if (Input::jpressed(GLFW_KEY_ESCAPE)) {
            Window::setShouldClose(true);
        }
        if (Input::jpressed(GLFW_KEY_TAB)) {
            Input::toggleCursor();
        }
        
        if (Input::pressed(GLFW_KEY_W)) {
            camera->position += camera->front * deltaTime * speed;
        }
        if (Input::pressed(GLFW_KEY_S)) {
            camera->position -= camera->front * deltaTime * speed;
        }
        if (Input::pressed(GLFW_KEY_A)) {
            camera->position -= camera->right * deltaTime * speed;
        }
        if (Input::pressed(GLFW_KEY_D)) {
            camera->position += camera->right * deltaTime * speed;
        }

        if (Input::_cursor_locked) {
            camX += -Input::deltaX / Window::height;
            camY += -Input::deltaY / Window::height;
            if (camY < -radians(89.0f)) camY = -radians(89.0f);
            if (camY > radians(89.0f)) camY = radians(89.0f);

            camera->rotation = mat4(1.0f);
            camera->rotate(camY, camX, 0);
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        voxshader->use();
        
        effect1->draw(deltaTime);
        voxshader->uniformMatrix("projview", camera->getProjection()*camera->getView());

        appleobj->draw();
        vec3 end;
        vec3 norm;
        vec3 iend;
        if (Input::jclicked(GLFW_MOUSE_BUTTON_1)) {
            if (appleobj->raycast(camera->getPosition(), camera->front, 10.0f, end, norm, iend) ) {
                //std::cout << "Obj" << std::endl;
                //chunks->set((int)(iend.x)+(int)(norm.x), (int)(iend.y)+(int)(norm.y), (int)(iend.z)+(int)(norm.z), 2);
                //objs[ind] = new GameObject(renderer, *nullvox, voxshader);
                appleobj->setVisible(false);
                //objs[ind]->setPosition(vec3((int)(iend.x)+(int)(norm.x), (int)(iend.y)+(int)(norm.y), (int)(iend.z)+(int)(norm.z)));
            }
        }

        // for(int i = 0; i < ind; i++) {
        //     objs[i]->draw();
        // }

        // std::cout << camera->getPosition().x << " " << camera->getPosition().y   << " " << camera->getPosition().z << " " << std::endl;
        // nullobj->draw();

        crosshairShader->use();
        crosshair->draw(GL_LINES);    
        Window::swapBuffers();
        Input::pullEvents();
    }

    Window::exit();
    
    delete voxshader;
    delete crosshairShader;

    delete applevox;

    delete effect1;
    //delete[] objs;

    //delete appleobj;
    //delete apple1obj;
    //delete waterobj;

    delete renderer;
    return 0;
}