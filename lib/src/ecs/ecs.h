#pragma once

#include <typeindex>
#include <memory>
#include <stdexcept>

#include "entitymanager.h"

#include "transform.h"
#include "rigidbody.h"
#include "collider.h"
#include "render.h"
#include "script.h"
#include "name.h"

#include "transformsystem.h"
#include "physicssystem.h"
#include "physicstotransformlink.h"
// #include "inputsystem.h"
#include "rendersystem.h"
// #include "scriptsystem.h"

#include "vendor/json.hpp"
using json = nlohmann::json;

class ECSManager {
public:
    //! Systems
    // InputSystem inputSystem;
    TransformSystem transformSystem;
    PhysicsSystem physicsSystem;
    PhysicsToTransformLink physicstransformlink;
    RenderSystem renderSystem;

    //!-----------------------------

    Entity createEntity() {
        return entities.createEntity();
    }

    template<typename T, typename... Args>
    void addComponent(Entity entity, Args&&... args) {
        auto& storage = getOrCreateStorage<T>();
        storage[entity] = T(std::forward<Args>(args)...);
    }

    template<typename T>
    void removeComponent(Entity entity) {
        auto& storage = getOrCreateStorage<T>();
        storage.erase(entity);
    }

    template<typename T>
    T& getComponent(Entity entity) {
        auto& storage = getOrCreateStorage<T>();
        auto it = storage.find(entity);
        if (it == storage.end()) {
            throw std::runtime_error("Component not found for entity");
        }
        return it->second;
    }

    template<typename T>
    bool hasComponent(Entity entity) {
        auto& storage = getOrCreateStorage<T>();
        return storage.find(entity) != storage.end();
    }

    void destroy(Entity entity) {
        auto & transforms = getOrCreateStorage<Transform>();
        auto &rigidbodies = getOrCreateStorage<RigidBody>();
        auto &colliders = getOrCreateStorage<Collider>();
        auto &scripts = getOrCreateStorage<Script>();
        auto &renders = getOrCreateStorage<Render>();

        transforms.erase(entity);
        rigidbodies.erase(entity);
        colliders.erase(entity);
        scripts.erase(entity);
        renders.erase(entity);
        
        entities.removeEntity(entity);
    }

    float fixedDelta = 1.0f / 60.0f; // 60 FPS физики
    float accumulator = 0.0f;

    void update(float deltaTime) {
        auto &transforms = getOrCreateStorage<Transform>();
        auto &rigidbodies = getOrCreateStorage<RigidBody>();
        auto &colliders = getOrCreateStorage<Collider>();
        auto &scripts = getOrCreateStorage<Script>();
        auto &renders = getOrCreateStorage<Render>();

        accumulator += deltaTime;
        while (accumulator >= fixedDelta) {
            physicsSystem.update(deltaTime);
            physicstransformlink.update(transforms, rigidbodies);
            accumulator -= fixedDelta;
        }

        transformSystem.update(transforms);

        renderSystem.update(transforms, renders);
    }

    const EntityManager& get_entities() const {
        return entities;
    }

    json serialize(const Entity entity) {
        auto &names = getOrCreateStorage<Name>();
        auto &transforms = getOrCreateStorage<Transform>();
        auto &rigidbodies = getOrCreateStorage<RigidBody>();
        auto &colliders = getOrCreateStorage<Collider>();
        auto &scripts = getOrCreateStorage<Script>();
        auto &renders = getOrCreateStorage<Render>();
        json j;

        Name& name = names[entity];
        Transform& transform = transforms[entity];
        RigidBody& rigidbody = rigidbodies[entity];
        Collider& collider = colliders[entity];
        Script& script = scripts[entity];
        Render& render = renders[entity];

        j["id"].push_back({{"id", entity}});
        if (hasComponent<Name>(entity)) {
            j["id"].push_back({{"name", name.name}});
        }
        if (hasComponent<Transform>(entity)) {
            j["id"].push_back({
                {"transform", {
                    {"position", {
                        {"x", transform.position.x}, 
                        {"y", transform.position.y}, 
                        {"z", transform.position.z}
                    }},
                    {"rotation", {
                        {"x", transform.rotation.x}, 
                        {"y", transform.rotation.y}, 
                        {"z", transform.rotation.z},
                        {"w", transform.rotation.w}
                    }},
                    {"scale", {
                        {"x", transform.scale.x}, 
                        {"y", transform.scale.y}, 
                        {"z", transform.scale.z}
                    }}
                }}
            });
        }
        if (hasComponent<RigidBody>(entity)) {
            j["id"].push_back({
                {"rigidbody", {
                    {"mass", rigidbody.body->getMass()},
                    {"type", rigidbody.body->getType()},
                    {"position", {
                        {"x", rigidbody.body->getTransform().getPosition().x},
                        {"y", rigidbody.body->getTransform().getPosition().y},
                        {"z", rigidbody.body->getTransform().getPosition().z}
                    }}
                }}
            });
        }
        if (hasComponent<Collider>(entity)) {
            rp3d::CollisionShape* colshape = colliders[entity].collider->getCollisionShape();
            j["id"].push_back({
                {"collider", {
                    {"transform", {
                        {"position", {
                            {"x", collider.collider->getLocalToBodyTransform().getPosition().x},
                            {"y", collider.collider->getLocalToBodyTransform().getPosition().y},
                            {"z", collider.collider->getLocalToBodyTransform().getPosition().z}
                        }},
                        {"rotation", {
                            {"x", collider.collider->getLocalToBodyTransform().getOrientation().x},
                            {"y", collider.collider->getLocalToBodyTransform().getOrientation().y},
                            {"z", collider.collider->getLocalToBodyTransform().getOrientation().z},
                            {"w", collider.collider->getLocalToBodyTransform().getOrientation().w}
                        }},
                    }},
                    {"type", colshape->getName()},
                    {"size", colshape->to_string()},
                    {"material", {
                        {"bounciness", collider.collider->getMaterial().getBounciness()},
                        {"frictioncoef", collider.collider->getMaterial().getFrictionCoefficient()},
                        {"massdensity", collider.collider->getMaterial().getMassDensity()}
                    }}
                }}
            });
        }
        if (hasComponent<Render>(entity)) {
            j["id"].push_back({
                {"modelName", render.modelName},
                {"visible", render.visible}
            });
        }

        std::ofstream file("test.json");
        if (!file.is_open()) {
            std::cerr << "Cannot open file for writing: " << "test.json" << "\n";
        }
        file << j.dump(4); // красивый отступ
        file.close();
        return j;
    }

private:
    EntityManager entities;
    std::unordered_map<std::type_index, std::shared_ptr<void>> storages;

    template<typename T>
    std::unordered_map<Entity, T>& getOrCreateStorage() {
        auto type = std::type_index(typeid(T));
        auto it = storages.find(type);

        if (it == storages.end()) {
            auto storage = std::make_shared<std::unordered_map<Entity, T>>();
            storages[type] = storage;
            return *storage;
        }

        return *std::static_pointer_cast<std::unordered_map<Entity, T>>(it->second);
    }
};