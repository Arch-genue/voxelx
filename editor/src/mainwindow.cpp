#include "mainwindow.h"
#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "window/window.h"
#include "graphics/renderer.h"

#include "./ui_mainwindow.h"
//#include "include/loaders/resourcemanager.h"
//#include "include/gamesystems/gamemanager.h"
//#include "include/voxels/voxelmodel.h"
//#include "include/gamesystems/camera.h"

//#include "include/graphics/types/octotree.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    //! Setup opengl background
    Window::sky = glm::vec3(0.5f, 0.5f, 0.5f);
};

MainWindow::~MainWindow() {
    delete ui;
}

