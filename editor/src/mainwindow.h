#pragma once

#include <QMainWindow>

#include "gamesystems/gamemanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void afterGLInit();

private:
    Ui::MainWindow *ui;

    GameManager* _gm;
};