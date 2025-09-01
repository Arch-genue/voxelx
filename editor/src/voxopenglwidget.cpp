#define GLEW_STATIC
#include <GL/glew.h>

#include "voxopenglwidget.h"

#include "window/window.h"
#include "utilities/logger.hpp"

#include "loaders/resourcemanager.h"

#include <chrono>


void VoxOpenGLWidget::initializeGL() {
    Window::createContext();
    Window::_glInit();

    ResourceManager::loadShaders();
    ResourceManager::loadTextures();
    ResourceManager::loadModels();
    ResourceManager::loadFonts();

    FT_Face face = ResourceManager::getFont("arial");
    _textMesh = new TextMesh(face);
    
    _ang = 0.0f;

    _camera = new Camera(glm::vec3(3, 1, 0), glm::radians(150.0f));
    Renderer::addCamera(_camera);
    GameObject *appleobj = new GameObject(gm, "player", ResourceManager::getModel("apple"), glm::vec3(0, 10, 5));
    appleobj->attachCamera(_camera, glm::vec3(0, 50, 0)); //! Attach camera to appleobj, apple hidden
    // appleobj->setPhysics(DYNAMIC_PHYSICS);
    appleobj->setVisible(true);
    gm->addGameObject(appleobj);
    _camera->setPosition(appleobj->getPosition() + glm::vec3(0, 3, 0));

    emit initialized();
}

void VoxOpenGLWidget::paintGL() {
    // auto newTimePoint = std::chrono::system_clock::now();
    // auto dtMsec = std::chrono::duration_cast<std::chrono::milliseconds>(newTimePoint - lastTimePoint);
    
    // static auto lastTimePoint = newTimePoint;
    static auto deltaTime = 0.001f;// * float(dtMsec.count());
    
    Window::_glClear();


    Light light;
    light.position = glm::vec3(0, 0, 0); //lightobj->getPosition();
    light.direction = _camera->getTarget();
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

    gm->UpdatePhysics(0.01f);

    gm->Update(light);

    ResourceManager::getShader("font")->use();
    ResourceManager::getShader("font")->uniformMatrix("projection", glm::ortho(0.0f, (float)Window::width, 0.0f, (float)Window::height));
    ResourceManager::getShader("font")->uniformVec3("textColor", glm::vec3(0.65f, 1.0f, 1.0f));
    _textMesh->draw("VoxelX", 5.0f, (float)Window::height-20.0f, 0.4f);

    ResourceManager::getShader("font")->uniformVec3("textColor", glm::vec3(0.1f, 0.5f, 0.1f));
    // _textMesh->draw("Debug mode", Window::width / 2 - 80.0f, Window::height-20.0f, 0.5f);
    _textMesh->draw("FPS: " + std::to_string((int)(1.0f / deltaTime)), 5.0f, Window::height-50.0f, 0.4f);
    
    ResourceManager::getShader("font")->uniformVec3("textColor", glm::vec3(1.0f, 0.0f, 0.3f));
    _textMesh->draw("> ", Window::width / 2, Window::height-20.0f, 0.4f);

    update();
    emit updated();
}

void VoxOpenGLWidget::resizeGL(int w, int h) {
    // qDebug() << "resizeGL" << w << " " << h;
    Window::resizeContext(w, h);
}