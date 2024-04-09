#ifndef VOXOPENGLWIDGET_H
#define VOXOPENGLWIDGET_H

#include <QOpenGLWidget>

#include "window/window.h"
#include "graphics/textmesh.h"
#include "gamesystems/gamemanager.h"

class VoxOpenGLWidget : public QOpenGLWidget {
    Q_OBJECT
private:
    TextMesh* _textMesh;

public:
    VoxOpenGLWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent) {}
    GameManager *gm;

    //! Temporary
    float _ang = 0.0f;

    Camera* _camera;

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    // void keyPressEvent(QKeyEvent *event) override;

signals:
    void initialized();
    void updated();

};

#endif // VOXOPENGLWIDGET_H
