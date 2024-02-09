/**
 * @file camera.h
 * @author Vlad Kartsaev
 * @brief Camera implementation
 * @version 0.5
 * @date 2023-04-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <glm/glm.hpp>

/**
 * @brief Класс игровой камеры
 * 
 */
class Camera {
    public:
        Camera(glm::vec3 position, float fov);
        ~Camera();

        void rotate(glm::vec2 rotation, float z);

        void setPosition(glm::vec3 position);
        glm::vec3 getPosition();

        void setRotation(glm::mat4 rotation);
        glm::mat4 getRotation();

        void setFOV(float fov);
        float getFOV();

        void setZoom(float zoom);
        float getZoom();

        void setPerspective(bool perspective);
        bool getPerspective();

        void setFlipped(bool flipped);
        bool getFlipped();

        glm::vec3 getFrontVector();
        glm::vec3 getUpVector();
        glm::vec3 getRightVector();

        glm::mat4 getProjection();
        glm::mat4 getView();
    private:
        void _updateVectors();
        
        glm::vec3 _front;
        glm::vec3 _up;
        glm::vec3 _right;
        glm::vec3 _dir;

        glm::vec3 _position;
        glm::mat4 _rotation;

        float _fov;
        float _zoom;
        bool _perspective = true;
        bool _flipped = false;
        float _aspect = 0.0f;
};