#include "mainwindow.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "loaders/resourcemanager.h"
#include "window/window.h"
#include "graphics/renderer.h"

#include "./ui_mainwindow.h"
#include "settings.h"

#include "voxels/voxelmodel.h"
#include "gamesystems/camera.h"

#include "graphics/types/octotree.h"

#define GLEW_STATIC
#include <GL/glew.h>

// #include "vglwidget.h"
#include "voxopenglwidget.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QSplitter>

#include <QFile>

#include "vsettings.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    vLogger::setLogLevel(LOGLEVEL::ERROR);
    VSettings vsettings("test");
    vsettings.save();

    //данные параметры используются QSettings для определения куда сохранять конфигурацию
    QCoreApplication::setOrganizationName("DnDPrograms");
    QCoreApplication::setApplicationName("VoxelX");

    // Settings::setSections(QStringList() << "General" << "Layout");
    // Settings::setSections(QStringList() << "LAYWINDOWSIZE" << "HORSPLITSIZE" << "VERSPLITSIZE");
    
    // Settings::loadConfig("config/default.cfg");

    // //чтение из секции General
    // QStringList horsplitsize = Settings::getSplitString(Settings::HORSPLITSIZE, Settings::Layout);
    // QStringList vertsplitsize = Settings::getSplitString(Settings::HORSPLITSIZE, Settings::Layout);
    // // .split(" ");
    // // QString versplitsize = Settings::get(Settings::VERSPLITSIZE, Settings::Layout).toString().split(" ");
    // // qDebug() << horsplitsize;
    // ui->horSplitterGL->setSizes(QList<int>() << horsplitsize[0].toInt() << horsplitsize[1].toInt());
    // ui->splitter_2->setSizes(QList<int>() << vertsplitsize[0].toInt() << vertsplitsize[1].toInt());

    // qDebug() << "Login: " << horsplitsize[0].toInt() << " " << horsplitsize[1].toInt();

    connect(ui->glwidget, &VoxOpenGLWidget::initialized, this, &MainWindow::afterGLInit);
    connect(ui->glwidget, &VoxOpenGLWidget::updated, this, &MainWindow::afterGLUpdated);
    connect(ui->dbgButton, &QPushButton::clicked, this, &MainWindow::debugBtnClicked);
    connect(ui->actionSave_layout, &QAction::changed, this, &MainWindow::saveLayout);

    //! Setup opengl background
    Window::sky = glm::vec3(0.5f, 0.5f, 0.5f);

    Renderer::init(4096 * 4096);

    ResourceManager::init("../res/");
    
    _gm = new GameManager();

    QTreeWidgetItem *headerItem = new QTreeWidgetItem();
    headerItem->setText(0, "Hierarchy");

    ui->treeWidget->setHeaderItem(headerItem);

    ui->treeWidget->setColumnCount(1);

    QTreeWidgetItem *itemGameobjects = new QTreeWidgetItem(ui->treeWidget);
    itemGameobjects->setText(0, "GameObjects");
    itemGameobjects->font(0).setPixelSize(15);

    QTreeWidgetItem *itemParticles = new QTreeWidgetItem(ui->treeWidget);
    itemParticles->setText(0, "Particle systems");

    ui->glwidget->gm = _gm;
};

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if(event->modifiers() == Qt::ControlModifier){ // Whether to press the Ctrl key
        if(event->key() == Qt::Key_M) // Whether to press the M key
            setWindowState(Qt::WindowMaximized); // window maximized
    }
    else QWidget::keyPressEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    std::cout << "Press" << std::endl;
}
void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    QPoint position = event->pos();
    qDebug() << position.x() << " " << position.y();
}

void MainWindow::afterGLInit() {
    GameObject *appleobj1 = new GameObject(_gm, "apple", ResourceManager::getModel("apple"), glm::vec3(0, 15, 10));
    // appleobj1->setPhysics(DYNAMIC_PHYSICS);

    _gm->addGameObject(appleobj1);
    
    for (int i = 0; i < 100; i++) {
        GameObject* gameObject = new GameObject(_gm, std::to_string(i), ResourceManager::getModel("apple"), glm::vec3(1.0f * i, 0.0f, 0.0f));
        _gm->addGameObject(gameObject);
    }
}

void MainWindow::afterGLUpdated() {
    for (GameObject* &gmobj : _gm->getGameObjects()) {
        // std::cout << gmobj->getID() << std::endl;
        QTreeWidgetItem* item = ui->treeWidget->topLevelItem(0);
        bool flag = false;
        for (int i = 0; i < item->childCount(); i++) {
            QTreeWidgetItem* child = item->child(i);
            if (child->text(0) == QString::number(gmobj->getID())) {
                flag = true;
            }
        }
        if (!flag) {
            QTreeWidgetItem* child1 = new QTreeWidgetItem(item);

            child1->setText(0, QString::number(gmobj->getID())); // gmobj->getName()
        }
    }
}

void MainWindow::debugBtnClicked() {
    qDebug() << ui->horSplitterGL->sizes()[0] << " " << ui->horSplitterGL->sizes()[1];
}

void MainWindow::saveLayout() {
    qDebug() << "SIZE: " << QString::number(ui->horSplitterGL->sizes()[0]) << " " << ui->horSplitterGL->sizes()[0] << " " << ui->horSplitterGL->sizes()[1];
    // Settings::set(Settings::HORSPLITSIZE1, Settings::Layout) = "QString::number(ui->horSplitterGL->sizes()[0]).toStdString()";
    // Settings::set(Settings::HORSPLITSIZE2, Settings::Layout) = "asdfsd"; // = QString::number(ui->horSplitterGL->sizes()[1]);
}


MainWindow::~MainWindow() {
    delete ui;
}

