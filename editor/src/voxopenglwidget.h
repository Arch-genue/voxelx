#ifndef VOXOPENGLWIDGET_H
#define VOXOPENGLWIDGET_H

#include <QOpenGLWidget>

class VoxOpenGLWidget : public QOpenGLWidget {
public:
    VoxOpenGLWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent) {}

protected:
    void initializeGL() override;
    void paintGL() override;

//private:
//    void createGLContext()
//    {
//        // Ваш код для создания контекста OpenGL
//        // Например:
//        // QOpenGLContext *glContext = new QOpenGLContext;
//        // glContext->setFormat(requestedFormat());
//        // glContext->create();
//        // glContext->makeCurrent(this);
//    }
};

#endif // VOXOPENGLWIDGET_H
