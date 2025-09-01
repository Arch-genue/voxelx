/**
 * @file gameobject.h
 * @author Vlad Kartsaev
 * @brief Game object implementation
 * @version 0.1
 * @date 2024-02-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

// #include <iostream>
#include <glm/glm.hpp>

#include "physics/physicsobject.h"
#include "transformobject.h"
#include "modelobject.h"
#include "gamesystems/gamemanager.h"

#include "utilities/vtype.hpp"

class MeshModel;
class Shader;
class PhysicsObject;
class GameManager;
class Camera;

/**
 * @brief Класс описывающий игровой объект с физической моделью и графическим отображением
 * 
 */
class GameObject : public TransformObject, public ModelObject {
private:
    //? OBJECTS
    Camera* _camera;

    uint _id;
    std::string _name;
    
    glm::vec3 _campos;

    //! Voxel Physics
    std::unique_ptr<PhysicsObject> _physicsobject;

    vtype::fndvector<std::string> _scripts;
    
public:
    GameObject(const std::string& name, MeshModel* mesh_model, const glm::vec3& position);
    ~GameObject() = default;

    void setID(uint id);
    uint getID() const { return _id; };

    void setName(const std::string& name) { _name = name; };
    const std::string& getName() const { return _name; };

    void onTransformed() override;

    void attachScript(const std::string& name);

    /**
     * @brief Получить указатель на PhysicsObject
     * 
     * @return PhysicsObject* Указатель на PhysicsObject
     */
    PhysicsObject* getPhysicsObject() { return _physicsobject.get(); }

    /**
     * @brief Задать позицию TransformObject и PhysicsObject
     * 
     * @param position Вектор новой позиции объекта
     */
    void setPosition(const glm::vec3& position) override;

    /**
     * @brief Назначить объект Camera игровому объекту
     * 
     * @param camera Объект Camera
     * @param stdpos Стандартное положение камеры
     */
    void attachCamera(Camera* camera, const glm::vec3& stdpos = glm::vec3(0));
    void detachCamera() { _camera = nullptr; }
    Camera *getCamera() const { return _camera; }

    const vtype::fndvector<std::string>& get_scripts() { return _scripts; }
};