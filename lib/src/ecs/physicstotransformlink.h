#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class PhysicsToTransformLink {
public:
    void update(std::unordered_map<Entity, Transform>& transforms, std::unordered_map<Entity, RigidBody>& rigidbodies) {
        for (auto& [entity, rigidBody] : rigidbodies) {
            if (transforms.find(entity) == transforms.end()) continue;

            auto& transform = transforms[entity];
            rp3d::Transform t = rigidBody.body->getTransform();

            transform.position = glm::vec3(t.getPosition().x, t.getPosition().y, t.getPosition().z);

            glm::quat q(t.getOrientation().w, t.getOrientation().x, t.getOrientation().y, t.getOrientation().z);
            transform.rotation = q;

            // transform.modelMatrix = translation * rotation * scale;
        }
    }
};