#pragma once

using Entity = uint32_t;

struct EntityManager {
    Entity nextId = 1;

    Entity createEntity() {
        return nextId++;
    }
};