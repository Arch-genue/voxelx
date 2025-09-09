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

#include "transformsystem.h"
#include "physicssystem.h"
#include "physicstotransformlink.h"
// #include "inputsystem.h"
#include "rendersystem.h"
// #include "scriptsystem.h"

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

    void update() {
        auto& transforms = getOrCreateStorage<Transform>();
        auto& rigidbodies = getOrCreateStorage<RigidBody>();
        // auto &renders = getOrCreateStorage<Render>();
        auto &scripts = getOrCreateStorage<Script>();
        auto &renders = getOrCreateStorage<Render>();

        physicsSystem.update();
        physicstransformlink.update(transforms, rigidbodies);
        transformSystem.update(transforms);

        renderSystem.update(transforms, renders);
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