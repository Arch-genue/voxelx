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

#include <vector>
#include <random>

#define MODELSIZE 1.0f

int WIDTH = 1024;
int HEIGHT = 768;

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

const int SIZE = 10;

void generateDungeon(int dungeon[SIZE][SIZE]) {
    srand(time(NULL));
    for (int x = 0; x < SIZE; x++) {
        for (int z = 0; z < SIZE; z++) {
            if (x == 0 || z == 0 || x == SIZE - 1 || z == SIZE - 1) {
                dungeon[x][z] = 1; // Границы подземелья
            }
            else if (rand() % 100 < 45) {
                dungeon[x][z] = 1; // Стена
            }
            else {
                dungeon[x][z] = 0; // Пустота
            }
        }
    }
}

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
    //std::vector<voxel_m> voxs;
    //watervox->voxels = voxs;
    for (size_t y = 0; y < watervox->m_size.y; y++) {
        for (size_t z = 0; z < watervox->m_size.z; z++) {
            for (size_t x = 0; x < watervox->m_size.x; x++) {
                voxel_m vox;
                vox.position = vec3(x, y, z);
                vox.clr = vec4(0.0f, 0.0f, 1.0f, 0.5f);
                watervox->voxels.push_back(vox);
            }
        }
    }
    //std::mt19937 rng(std::random_device{}());
    //std::uniform_real_distribution<float> clr_generator(0.1f, 0.9f);
    // _voxels* wallvox = new _voxels;
    // wallvox->m_size = vec3(30, 50, 30);
    //std::vector<voxel_m> voxs;
    //wallvox->voxels = voxs;
    // float clr;
    // for (size_t y = 0; y < wallvox->m_size.y; y++) {
    //     for (size_t z = 0; z < wallvox->m_size.z; z++) {
    //         for (size_t x = 0; x < wallvox->m_size.x; x++) {
    //             voxel_m vox;
    //             clr = clr_generator(rng);
    //             vox.position = vec3(x, y, z);
    //             vox.clr = vec4(clr, clr, clr, 1.0f);
    //             vox.visible = false;
    //             if (
    //                 ( x == 0 || x == wallvox->m_size.x-1 ) ||
    //                 ( y == 0 || y == wallvox->m_size.y-1 ) ||
    //                 ( z == 0 || z == wallvox->m_size.z-1 )
    //             ) vox.visible = true;
    //             wallvox->voxels.push_back(vox);
    //         }
    //     }
    // }

    Renderer* renderer = new Renderer(1024*1024*10);

    //GameObject* wallobj = new GameObject(renderer, *wallvox, voxshader);
    GameObject* appleobj = new GameObject(renderer, *applevox, voxshader);
    // GameObject* apple1obj = new GameObject(renderer, *applevox, voxshader);
    // GameObject* waterobj = new GameObject(renderer, *watervox, voxshader);

    //int dungeon[SIZE][SIZE];
    //generateDungeon(dungeon);
    //printDungeon(dungeon);
    
    glClearColor(0.6f, 0.6f, 0.6f, 1);
    
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Mesh* crosshair = new Mesh(new _voxels, vertices, 4, attrs);
    Camera* camera = new Camera(vec3(3, 0, 0), radians(70.0f));

    float lastTime = glfwGetTime();
    float deltaTime = 0.0f;

    float camX = 0.0f;
    float camY = 0.0f;

    float speed = 10;

    //!TESTING
    VoxelParticles* effect = new VoxelParticles(30, renderer, voxshader);
    effect->setType(EFFECT_VOMIT);
    effect->setPosition(vec3(0.0f, 0.0f, 0.0f));

    VoxelParticles* effect1 = new VoxelParticles(10, renderer, voxshader);
    effect1->setType(EFFECT_FLAME);
    effect1->setPosition(vec3(40.0f, 0.0f, 0.0f));

    VoxelParticles* effect2 = new VoxelParticles(10, renderer, voxshader);
    effect2->setType(EFFECT_WATER);
    effect2->setPosition(vec3(0.0f, 0.0f, -20.0f));
    bool test = false;

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
        
        effect1->draw(deltaTime);
        

        //wallobj->draw();

        // for (int x = 0; x < SIZE; x++) {
        //     for (int z = 0; z < SIZE; z++) {
        //         if (dungeon[x][z] == 1) {
        //             //std::cout << "#";
        //             wallobj->draw();
        //             wallobj->setPosition(vec3(x*30, 0, z*30));
        //         }
        //     }
        // }
        appleobj->draw();
        vec3 end;
        vec3 norm;
        vec3 iend;
        // if ( appleobj->rayCast(camera->position, camera->front, 3.0f, end, norm, iend) ) {
        //     test = true;
        //     effect1->setPosition(vec3(camera->front.x, camera->front.y + 10.0f, camera->front.z));
        // }
        if (Input::jclicked(GLFW_MOUSE_BUTTON_1)) {
            //std::cout << camera->position.x << " " << camera->front.x << std::endl;
            if (appleobj->rayCast(camera->position, camera->front, 3.0f, end, norm, iend)) {
                test = true;
                effect1->setPosition(vec3(camera->front.x, camera->front.y + 10.0f, camera->front.z));
            } else effect1->setPosition(vec3(40.0f, 0, 0));
        }

        crosshairShader->use();
        crosshair->draw(GL_LINES);
        
        Window::swapBuffers();
        Input::pullEvents();
    }

    Window::exit();
    delete voxshader;
    delete crosshairShader;

    delete applevox;
    delete watervox;

    delete effect;
    delete effect1;

    //delete appleobj;
    //delete apple1obj;
    //delete waterobj;

    delete renderer;
    return 0;
}