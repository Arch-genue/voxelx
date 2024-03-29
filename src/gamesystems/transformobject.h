/**
 * @file transformobject.h
 * @author Vlad Kartsaev
 * @brief TransformObject class definition
 * @version 0.1
 * @date 2024-02-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define DEFAULT_SCALE 0.1f

/**
 * @brief Абстрактный класс описывающий пространственный объект и его свойства
 * 
 */
class TransformObject {
protected:
    glm::mat4 _modelmatrix;

    glm::vec3 _position;
    glm::vec3 _scaling;
    glm::vec3 _sizes;

    glm::vec3 _rotationAxis;
    float _rotationAngle;

    //* TEMPORARY
    bool _visible;

private:
    glm::mat4 _scalematrix;
    glm::mat4 _positionmatrix;
    glm::mat4 _rotatematrix;

public:
    TransformObject(/* args */);
    ~TransformObject();

    void update();
    virtual void draw();

    /**
     * @brief Перемещение объекта
     * 
     * @param val Значение перемещения
     * @param vector Вектор направления
     */
    void translate(float val, glm::vec3 vector);
    /**
     * @brief Вращение объекта
     * 
     * @param val Значение вращения
     * @param vector Вектор вращения
     */
    void rotate(float val, glm::vec3 vector);
    /**
     * @brief Масштабирование объекта
     * 
     * @param val Значение масштабирования
     * @param vector вектор масштабирования 
     */
    void scale(float val, glm::vec3 vector);

    /**
     * @brief Задать позицию объекта
     * 
     * @param position Вектор позиции объекта
     */
    virtual void setPosition(glm::vec3 position);
    /**
     * @brief Получить вектор позиции объекта
     * 
     * @return glm::vec3 Позиция объекта
     */
    glm::vec3 getPosition();

    void setRotationAroundPoint(float angle, glm::vec3 rotation, glm::vec3 point);

    /**
     * @brief Задать вращение объекта
     * 
     * @param angle Угол вращения
     * @param rotation Вектор вращения
     */
    void setRotation(float angle, glm::vec3 rotation);
    /**
     * @brief Получить значение вращения объекта
     * 
     * @param angle Ссылка на переменную угла вращения
     * @param rotation Ссылка на переменную вектора вращения
     */
    void getRotation(float &angle, glm::vec3 &rotation);

    void setScaling(glm::vec3 scaling);
    glm::vec3 getScaling();

    void setSize(glm::vec3 size);
    glm::vec3 getSize();

    /**
     * @brief Задать видимость объекта
     * 
     * @param visible Видимость объекта
     */
    void setVisible(bool visible);
    bool isVisible();

    /**
     * @brief Получить матрицу модели объекта
     * 
     * @return glm::mat4 Матрица модели объекта
     */
    glm::mat4 getMatrix();
};
