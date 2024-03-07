#include "voxopenglwidget.h"

#include "window/window.h"

void VoxOpenGLWidget::initializeGL() {
    Window::createContext();
    Window::_glInit();
}

void VoxOpenGLWidget::paintGL() {
    Window::_glClear();
}