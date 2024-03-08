#include "mainwindow.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "loaders/resourcemanager.h"
#include "window/window.h"
#include "graphics/renderer.h"

#include "./ui_mainwindow.h"

#include "voxels/voxelmodel.h"
#include "gamesystems/camera.h"

#include "graphics/types/octotree.h"


#define GLEW_STATIC
#include <GL/glew.h>

// #include "vglwidget.h"
#include "voxopenglwidget.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->glwidget, &VoxOpenGLWidget::initialized, this, &MainWindow::afterGLInit);

    //! Setup opengl background
    Window::sky = glm::vec3(0.5f, 0.5f, 0.5f);

    Renderer::init(4096 * 4096);

    ResourceManager::init("../res/");
    
    _gm = new GameManager();
    
};

void MainWindow::afterGLInit() {
    GameObject* gameObject = new GameObject(ResourceManager::getModel("apple"), glm::vec3(0.0f, 0.0f, 0.0f));

    _gm->addGameObject(gameObject);
}

MainWindow::~MainWindow() {
    delete ui;
}

