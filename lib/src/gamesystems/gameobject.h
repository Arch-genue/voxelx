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

#include "../graphics/renderer.h"
#include "../physics/physics.h"
#include "transformobject.h"
#include "modelobject.h"
#include "../gamesystems/gamemanager.h"

class Mesh;
class Shader;
class Renderer;
class PhysicsObject;
class GameManager;

/**
 * @brief Класс описывающий игровой объект с физической моделью и графическим отображением
 * 
 */
class GameObject : public TransformObject, public ModelObject {
private:
    //? OBJECTS
    Camera* _camera;

    /**
     * @brief Unique ID
     * 
     */
    uint _id;
    /**
     * @brief Unique name
     * 
     */
    std::string _name;
    
    //? GameManager global object
    GameManager* _gm;
    
    glm::vec3 _campos;

    //! Voxel Physics
    PhysicsObject* _physicsobject;
    
public:
    GameObject(GameManager* gm, std::string name, VoxelModel* model, glm::vec3 position);
    ~GameObject();

    void setID(uint id);
    uint getID() const { return _id; };

    void setName(std::string name) { _name = name; };
    std::string getName() const { return _name; };

    void onTransformed() override;

    /**
     * @brief Задать указатель на объект GameManager
     * 
     * @param gamemanager Указатель на объект GameManager
     */
    void setGameManager(GameManager* gamemanager);
    /**
     * @brief Получить указатель на объект GameManager
     * 
     * @return Shader* Указатель на объект GameManager или nullptr
     */
    GameManager* getGameManager();

    /**
     * @brief Получить указатель на PhysicsObject
     * 
     * @return PhysicsObject* Указатель на PhysicsObject
     */
    PhysicsObject* getPhysicsObject();

    /**
     * @brief Задать позицию TransformObject и PhysicsObject
     * 
     * @param position Вектор новой позиции объекта
     */
    void setPosition(glm::vec3 position) override;

    /**
     * @brief Назначить объект Camera игровому объекту
     * 
     * @param camera Объект Camera
     * @param stdpos Стандартное положение камеры
     */
    void attachCamera(Camera* camera, glm::vec3 stdpos = glm::vec3(0));
    void detachCamera();
};