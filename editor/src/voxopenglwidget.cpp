#define GLEW_STATIC
#include <GL/glew.h>

#include "voxopenglwidget.h"

#include "window/window.h"
#include "utilities/logger.h"

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

    
    emit initialized();
}

void VoxOpenGLWidget::paintGL() {
    // auto newTimePoint = std::chrono::system_clock::now();
    // auto dtMsec = std::chrono::duration_cast<std::chrono::milliseconds>(newTimePoint - lastTimePoint);
    
    // static auto lastTimePoint = newTimePoint;
    static auto deltaTime = 0.001f;// * float(dtMsec.count());
    
    Window::_glClear();

    ResourceManager::getShader("font")->use();
    ResourceManager::getShader("font")->uniformMatrix("projection", glm::ortho(0.0f, (float)Window::width, 0.0f, (float)Window::height));
    ResourceManager::getShader("font")->uniformVec3("textColor", glm::vec3(0.65f, 1.0f, 1.0f));
    _textMesh->draw("VoxelX", 5.0f, (float)Window::height-20.0f, 0.4f);

    ResourceManager::getShader("font")->uniformVec3("textColor", glm::vec3(0.1f, 0.5f, 0.1f));
    _textMesh->draw("Debug mode", Window::width / 2 - 80.0f, Window::height-20.0f, 0.5f);
    _textMesh->draw("FPS: " + std::to_string((int)(1.0f / deltaTime)), 5.0f, Window::height-50.0f, 0.4f);

    update();
}

void VoxOpenGLWidget::resizeGL(int w, int h) {
    qDebug() << "resizeGL" << w << " " << h;
    Window::resizeContext(w, h);
}