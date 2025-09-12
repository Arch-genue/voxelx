#pragma once

#include <set>

using Entity = uint32_t;

class EntityManager {
public:
    Entity createEntity() {
        _entities.insert(nextId);
        return nextId++;
    }

    void removeEntity(Entity entity) {
        _entities.erase(entity);
    }

    Entity get_id() const {
        return nextId;
    }

private:
    std::set<Entity> _entities;

    Entity nextId = 1;
};