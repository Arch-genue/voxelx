#pragma once

#include <glm/gtc/matrix_transform.hpp>

struct TransformSystem {
    void update(std::unordered_map<Entity, Transform>& transforms) {
        for (auto& [entity, transform] : transforms) {
            glm::mat4 model(1.0f);

            // порядок трансформаций: scale → rotate → translate
            model = glm::translate(model, transform.position);

            // вращение по осям
            model = glm::rotate(model, transform.rotation.x, glm::vec3(1, 0, 0));
            model = glm::rotate(model, transform.rotation.y, glm::vec3(0, 1, 0));
            model = glm::rotate(model, transform.rotation.z, glm::vec3(0, 0, 1));

            model = glm::scale(model, transform.scale);

            transform.modelMatrix = model;
        }
    }
};