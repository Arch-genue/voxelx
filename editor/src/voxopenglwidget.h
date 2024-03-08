#ifndef VOXOPENGLWIDGET_H
#define VOXOPENGLWIDGET_H

#include <QOpenGLWidget>

#include "window/window.h"
#include "graphics/textmesh.h"

class VoxOpenGLWidget : public QOpenGLWidget {
    Q_OBJECT
private:
    TextMesh* _textMesh;

public:
    VoxOpenGLWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent) {}

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

signals:
    void initialized();

};

#endif // VOXOPENGLWIDGET_H
