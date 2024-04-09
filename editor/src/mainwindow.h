#pragma once

#include <QMainWindow>

#include "gamesystems/gamemanager.h"

#include <QKeyEvent>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void afterGLInit();
    void afterGLUpdated();

    void debugBtnClicked();
    void saveLayout();

    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    Ui::MainWindow *ui;

    GameManager* _gm;
};