#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/ext.hpp>
using namespace glm;

#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/mesh.h"
//#include "graphics/voxelrenderer.h"
#include "graphics/vox_renderer.h"
#include "window.h"
#include "input.h"
#include "camera.h"
#include "loaders/png_loading.h"
#include "voxels/voxel.h"
//#include "voxels/chunk.h"
//#include "voxels/chunks.h"
#include "gameobject.h"
#include "entity.h"

#define MODELSIZE 1.0f

int WIDTH = 800;
int HEIGHT = 600;

float vertices[] = {
    // x    y
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

    //?Loaders
    // Shader* shader = load_shader("../res/shaders/main.glslv", "../res/shaders/main.glslf");
    // if (shader == nullptr) {
    //     std::cerr << "Failed to load shader\n";
    //     Window::exit();
    //     return 1;
    // }

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

    // Texture* texture = load_texture("../res/block.png");
    // if (texture == nullptr) {
    //     std::cerr << "Failed to load texture\n";
    //     //delete shader;
    //     Window::exit();
    //     return 1;
    // }

    VoxModel* goblinhead = new VoxModel("../res/models/goblin.voxtxt", false);
    VoxModel* goblintorso = new VoxModel("../res/models/goblintorso.voxtxt", false);

    VoxModel* goblinLegLeft = new VoxModel("../res/models/goblinLegLeft.voxtxt", false);
    VoxModel* goblinLegRight = new VoxModel("../res/models/goblinLegRight.voxtxt", false);

    VoxModel* goblinArmLeft = new VoxModel("../res/models/goblinArmLeft.voxtxt", false);
    
    VoxModel* watertest = new VoxModel("../res/models/watertest.voxtxt", true);
    VoxModel* null = new VoxModel("../res/models/null.voxtxt", false);

    //VoxModel* goblinArmRight = new VoxModel("../res/models/goblinArmRight.voxtxt");

    //VoxelRenderer renderer(1024*1024);
    ModelRenderer rend(1024*1024*10);

    Mesh* mesh1 = rend.render(goblinhead);
    Mesh* mesh2 = rend.render(goblintorso);
    
    Mesh* mesh3 = rend.render(goblinLegLeft);
    Mesh* mesh4 = rend.render(goblinLegRight);

    Mesh* mesh5 = rend.render(goblinArmLeft);
    Mesh* mesh6 = rend.render(watertest);
    Mesh* mesh7 = rend.render(watertest);

    Mesh* nullmesh = rend.render(null);

    mat4 mat(1.0f);
    GameObject obj1(&mat);
    GameObject obj2(&mat);

    GameObject obj3(&mat);
    GameObject obj4(&mat);

    GameObject obj5(&mat);
    GameObject obj6(&mat);
    GameObject obj7(&mat);
    
    glClearColor(0.6f, 0.6f, 0.6f,1);
    
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Mesh* crosshair = new Mesh(vertices, 4, attrs);
    Camera* camera = new Camera(vec3(10,1,5), radians(90.0f));

    float lastTime = glfwGetTime();
    float delta = 0.0f;

    float camX = 0.0f;
    float camY = 0.0f;

    float speed = 5;

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
        if (Input::jclicked(GLFW_MOUSE_BUTTON_1)) {
            //chunks->set((int)iend.x, (int)iend.y, (int)iend.z, 0);
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

        //*GOBLIN
        
        //Head
        obj1.setPosition(glm::vec3(100, 18, 30));
        obj1.setRotation(radians(-90.0f), vec3(1,0,0));
        obj1.draw(mesh1, voxshader); 

        //Torso
        obj2.setPosition(glm::vec3(100, 8, 30));
        obj2.setRotation(radians(-90.0f), vec3(1,0,0));
        obj2.draw(mesh2, voxshader);
        
        //Left leg
        obj3.setPosition(glm::vec3(101, 0, 24));
        obj3.setRotation(radians(-90.0f), vec3(1,0,0));
        obj3.draw(mesh3, voxshader);

        //Right leg
        obj3.setPosition(glm::vec3(101, 0, 28));
        obj3.setRotation(radians(-90.0f), vec3(1,0,0));
        obj3.translate(0.1f, vec3(1, 0 ,0));
        obj3.draw(mesh4, voxshader);

        obj3.setRotation(radians(-90.0f), vec3(1,0,0));
        obj3.setPosition(vec3(0, -3, 99));
        obj3.draw(mesh6, voxshader);

        obj3.setRotation(radians(0.0f), vec3(1,0,0));
        obj3.setPosition(vec3(0, 0, 0));
        obj3.draw(mesh6, voxshader);

        obj4.setPosition(vec3(0, 0, 97));
        obj4.draw(mesh7, voxshader);

        crosshairShader->use();
        crosshair->draw(GL_LINES);
        
        Window::swapBuffers();
        Input::pullEvents();
    }

    Window::exit();
    delete voxshader;
    delete crosshairShader;

    delete goblinhead;
    delete goblinArmLeft;
    delete goblintorso;
    delete goblinLegLeft;
    delete goblinLegRight;
    delete null;
    delete watertest;
    
    delete mesh1;
    delete mesh2;
    delete mesh3;
    delete mesh4;
    delete mesh5;
    delete mesh6;
    delete mesh7;
    delete nullmesh;
    return 0;
}