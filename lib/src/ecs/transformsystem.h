#pragma once

#include <glm/gtc/matrix_transform.hpp>

struct TransformSystem {
    void update(std::unordered_map<Entity, Transform>& transforms) {
        for (auto& [entity, transform] : transforms) {
            glm::mat4 model(1.0f);

            // Сначала масштаб
            model = glm::scale(model, transform.scale);

            // Потом вращение через кватернион
            model *= glm::toMat4(transform.rotation);

            // И наконец позиция
            model = glm::translate(glm::mat4(1.0f), transform.position) * model;

            transform.modelMatrix = model;
        }
    }
};