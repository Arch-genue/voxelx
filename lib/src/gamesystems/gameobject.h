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

#include <glm/glm.hpp>

#include "../graphics/renderer.h"
#include "../physics/physics.h"
#include <iostream>
#include "transformobject.h"

class Mesh;
class Shader;
class Renderer;
class PhysicsObject;
class GameManager;

/**
 * @brief Класс описывающий игровой объект с физической моделью и графическим отображением
 * 
 */
class GameObject : public TransformObject {
    public:
        GameObject(VoxelModel* model);
        GameObject(VoxelModel* model, glm::vec3 position);
        ~GameObject();

        void setID(uint id);
            uint getID() const { return _id; };

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
         * @brief Задать тип физической модели объекта
         * 
         * @param physics 
         */
        void setPhysics(PHYSICS physics);

        /**
         * @brief Отрисовка модели 
         * 
         */
        void draw() override;
        /**
         * @brief Просчет физики
         * 
         * @param deltaTime deltaTime
         */
        void updatePhysics(float deltaTime);

        /**
         * @brief Назначить объект Camera игровому объекту
         * 
         * @param camera Объект Camera
         * @param stdpos Стандартное положение камеры
         */
        void attachCamera(Camera* camera, glm::vec3 stdpos);

        /**
         * @brief Получить указатель на объект VoxelModel
         * 
         * @return Указатель на объект VoxelModel или nullptr
         */
        VoxelModel* getVoxelModel();

        bool raycast(glm::vec3 position, glm::vec3 dir, float maxDist, glm::vec3& end, glm::vec3& norm, glm::vec3& iend);

        void setMesh(Mesh* mesh);

        /**
         * @brief Получить указатель на объект Mesh
         * 
         * @return Mesh* Указатель на объект Mesh или nullptr
         */
        Mesh* getMesh();
    private:
        //? OBJECTS
        VoxelModel* _voxelmodel;
        Mesh* _mesh;
        Mesh* _boundingboxmesh;
        Mesh* _raymesh;
        Camera* _camera;

        uint _id;
        
        //? GameManager global object
        GameManager* _gm;
        
        glm::vec3 _campos;

        //! Voxel Physics
        PhysicsObject* _physicsobject;
};