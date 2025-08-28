#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>
#include "gamesystems/gamemanager.h"
#include "gamesystems/gameobject.h" // Твой класс игровых объектов

#include "scriptutils.h"

class ScriptSystem {
public:
    ScriptSystem(GameManager* gm): _gm(gm) {
        lua.open_libraries(
            sol::lib::base,
            sol::lib::math,
            sol::lib::table,
            sol::lib::string
        );
    }

    // Регистрируем GameObject в Lua
    void bindGameAPI() {
        lua.set_function("get_vec3", [](float x, float y, float z) { return ScriptUtils::get_vec3(x,y,z); });
        lua.set_function("get_vec3_val", [](float val) { return ScriptUtils::get_vec3(val); });

        lua.new_usertype<GameObject>("GameObject",
            "setPosition", &GameObject::setPosition,
            "getPosition", &GameObject::getPosition,
            "getName", &GameObject::getName,
            "getID", &GameObject::getID,
            "getPhysicsObject", &GameObject::getPhysicsObject
        );
        lua.new_usertype<PhysicsObject>("PhysicObject",
            "applyForce", &PhysicsObject::applyForce,
            "getRigidBody", &PhysicsObject::getRigidBody,
            "getCollider", &PhysicsObject::getCollider
        );

        lua.new_usertype<RaycastHit>("RaycastHit",
            "point", &RaycastHit::point,
            "normal", &RaycastHit::normal,
            "hitObject", &RaycastHit::hitObject
        );

        lua.new_usertype<PhysicsEngine>("PhysicsEngine",
            "raycast", &PhysicsEngine::raycast,
            "test", &PhysicsEngine::test
        );
        lua.new_usertype<glm::vec3>("vec3",
            sol::constructors<glm::vec3(float), glm::vec3(float, float, float)>(),
            "x", &glm::vec3::x,
            "y", &glm::vec3::y,
            "z", &glm::vec3::z
        ); // const float&, const float&, const float&
        
        // Передаём объект в Lua
        lua["physics"] = _gm->getPhysicsEngine();
    }

    // Загружаем скрипт с файла
    bool loadScript(const std::string& scriptName, const std::string& path) {
        try {
            sol::load_result script = lua.load_file(path);
            if (!script.valid()) {
                sol::error err = script;
                std::cerr << "[Lua Load Error] " << err.what() << std::endl;
                return false;
            }

            sol::protected_function pf = script;
            sol::protected_function_result result = pf();
            if (!result.valid()) {
                sol::error err = result;
                std::cerr << "[Lua Runtime Error] " << err.what() << std::endl;
                return false;
            }

            scripts[scriptName] = path;
            return true;
        } catch (const sol::error& e) {
            std::cerr << "[Lua Exception] " << e.what() << std::endl;
            return false;
        }
        // try {
        //     lua.script_file(path);
        //     scripts[scriptName] = path;
        //     return true;
        // } catch (const sol::error& e) {
        //     std::cerr << "[Lua Error]: " << e.what() << std::endl;
        //     return false;
        // }
    }

    // Вызов функции update в Lua
    void updateScript(const std::string& scriptName, GameObject* obj, float deltaTime) {
        try {
            sol::function updateFunc = lua[scriptName]["update"];
            if (updateFunc.valid()) {
                updateFunc(obj, deltaTime);
            }
        } catch (const sol::error& e) {
            std::cerr << "Lua runtime error: " << e.what() << std::endl;
        }
    }

    // Вызов всех скриптов, можно делать в цикле
    void updateAll(const std::vector<GameObject*>& objects, float deltaTime) {
        for (auto& [name, path] : scripts) {
            for (auto* obj : objects) {
                updateScript(name, obj, deltaTime);
            }
        }
    }

    sol::state& getLuaState() { return lua; }

private:
    GameManager* _gm;
    ScriptUtils* _utils;

    sol::state lua;
    std::unordered_map<std::string, std::string> scripts; // имя → путь к файлу
};