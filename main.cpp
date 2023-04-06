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
#include "graphics/voxelrenderer.h"
#include "graphics/vox_renderer.h"
#include "window.h"
#include "input.h"
#include "camera.h"
#include "loaders/png_loading.h"
#include "voxels/voxel.h"
#include "voxels/chunk.h"
#include "voxels/chunks.h"
#include "object.h"

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
    Shader* shader = load_shader("../res/shaders/main.glslv", "../res/shaders/main.glslf");
    if (shader == nullptr) {
        std::cerr << "Failed to load shader\n";
        Window::exit();
        return 1;
    }

    Shader* voxshader = load_shader("../res/shaders/voxel.glslv", "../res/shaders/voxel.glslf");

    Shader* crosshairShader = load_shader("../res/shaders/crosshair.glslv", "../res/shaders/crosshair.glslf");
    if (crosshairShader == nullptr) {
        std::cerr << "Failed to load crosshair shader\n";
        Window::exit();
        return 1;
    }

    Texture* texture = load_texture("../res/block.png");
    if (texture == nullptr) {
        std::cerr << "Failed to load texture\n";
        delete shader;
        Window::exit();
        return 1;
    }

    VoxModel* voxmodel = new VoxModel("../res/models/apple.voxtxt");
    //VoxModel* voxmodel1 = new VoxModel("res/models/bread.voxtxt");
    //VoxModel* voxmodel2 = new VoxModel("res/models/goblin.voxtxt");
    // if (voxmodel == nullptr) {
    //     std::cerr << "Failed to load voxel model\n";
    //     //delete voxmodel;
    //     Window::exit();
    //     return 1;
    // }

    //-----------

    Chunks* chunks = new Chunks(8, 1, 8);
    Mesh** meshes = new Mesh*[chunks->volume];
    for (size_t i = 0; i < chunks->volume; i++) 
        meshes[i] = nullptr;

    VoxelRenderer renderer(1024*1024);
    ModelRenderer rend(1024*1024);

    Mesh* mesh = rend.render(voxmodel);

    mat4 mat(1.0f);
    Object *objs[100];

    for (int i = 0; i < 100; i++)
        objs[i] = new Object(&mat);

    //mat4 mat1(1.0f);
    // for (int d = 0; d < 100; d++) {
    //     objd[d] = new Object(&mat);
    // }
    // //mat4 mat1(1.0f);
    // for (int g = 0; g < 100; g++) {
    //     objg[g] = new Object(&mat);
    // }
    
    glClearColor(0.6f,0.6f,0.6f,1);
    
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

    float rd,rb = 0;

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

        {
            vec3 end;
            vec3 norm;
            vec3 iend;
            Voxel* vox = chunks->rayCast(camera->position, camera->front, 5.0f, end, norm, iend);
            if (vox != nullptr) {
                if (Input::jclicked(GLFW_MOUSE_BUTTON_1)) {
                    chunks->set((int)iend.x, (int)iend.y, (int)iend.z, 0);
                }
                if (Input::jclicked(GLFW_MOUSE_BUTTON_2)) {
                    chunks->set((int)iend.x+(int)(norm.x), (int)iend.y+(int)(norm.y), (int)iend.z+(int)(norm.z), 2);
                }
            }
        }

        Chunk* closes[27];
        for (int i = 0; i < chunks->volume; i++) {
            Chunk* chunk = chunks->chunks[i];
            if (!chunk->modified) continue;
            chunk->modified = false;

            if (meshes[i] != nullptr) delete meshes[i];
            for (int d = 0; d < 27; d++) closes[d] = nullptr;
            
            for (int j = 0; j < chunks->volume; j++) {
                Chunk* other = chunks->chunks[j];

                int ox = other->x - chunk->x;
                int oy = other->y - chunk->y;
                int oz = other->z - chunk->z;

                if (abs(ox) > 1 || abs(oy) > 1 || abs(oz) > 1) continue;

                ox += 1;
                oy += 1;
                oz += 1;
                closes[(oy * 3 +oz) * 3 + ox] = other;
            }

            Mesh* mesh = renderer.render(chunk, (const Chunk**)closes);
            meshes[i] = mesh;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        voxshader->use();
        voxshader->uniformMatrix("projview", camera->getProjection()*camera->getView());
        for (int i = 0; i < 1; i++) {
            objs[i]->translate(glm::vec3(6*i, 0, 0));
            objs[i]->draw(mesh, voxshader);
        }

        //MAIN VOXELS
        mat4 model(1.0f);
        shader->use();
        shader->uniformMatrix("projview", camera->getProjection()*camera->getView());
        texture->bind();
        for (size_t i = 0; i < chunks->volume; i++) {
            Chunk* chunk = chunks->chunks[i];
            Mesh* mesh = meshes[i];
            model = translate(mat4(1.0f), vec3((chunk->x*CHUNK_W+0.5f)*MODELSIZE, (chunk->y*CHUNK_H+0.5f)*MODELSIZE, (chunk->z*CHUNK_D+0.5f)*MODELSIZE));
            //model = scale(model, vec3(MODELSIZE));

            shader->uniformMatrix("model", model);
            mesh->draw(GL_TRIANGLES);
        }
        
        crosshairShader->use();
        crosshair->draw(GL_LINES);
        
        Window::swapBuffers();
        Input::pullEvents();
    }

    Window::exit();
    delete shader;
    delete crosshairShader;
    delete texture;
    //delete voxmodel;
    //delete mesh;
    delete chunks;
    return 0;
}