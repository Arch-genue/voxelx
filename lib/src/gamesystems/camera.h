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

    void rotate(const glm::vec2& rotation, float z);

    void setPosition(glm::vec3 pos) { _position = pos / 10.0f; }
    glm::vec3 getPosition() const { return _position * 10.0f; }

    void setRotation(glm::mat4 rotation) { _rotation = rotation; }
    const glm::mat4& getRotation() const { return _rotation; }

    void setFOV(float fov) { _fov = fov; }
    float getFOV() const { return _fov; }

    void setZoom(float zoom) { _zoom = zoom; }
    float getZoom() const { return _zoom; }

    void setPerspective(bool perspective) { _perspective = perspective; }
    bool getPerspective() const { return _perspective; }

    void setFlipped(bool flipped) { _flipped = flipped; }
    bool getFlipped() const { return _flipped; }

    const glm::vec3& getTarget() const { return _front; }
    const glm::vec3& getUp() const { return _up; }
    const glm::vec3& getRight() const { return _right; }

    glm::mat4 getProjection(float aspect) const;
    glm::mat4 getView() const;
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