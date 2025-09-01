#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>
#include "gamesystems/gamemanager.h"
#include "gamesystems/gameobject.h"
#include "window/input.h"

#include "scriptutils.h"
#include "utilities/utils.h"

// Extensions
#include "gameobject_api.hpp"
#include "physics_api.hpp"
#include "input_api.hpp"
#include "logger_api.hpp"
#include "voxel_api.hpp"

class ScriptSystem {
private:
    ScriptSystem() {
        lua.open_libraries(
            sol::lib::base,
            sol::lib::math,
            sol::lib::table,
            sol::lib::string
        );

        bindGameAPI();
    }
    ~ScriptSystem() = default;

    sol::state lua;
    std::unordered_map<std::string, std::string> scripts;

public:
    // Singleton
    static ScriptSystem& instance() {
        static ScriptSystem inst;
        return inst;
    }

    void bindGameAPI() {
        LoggerApi::reg(lua);
        InputApi::reg(lua);
        GameObjectApi::reg(lua);
        PhysicsApi::reg(lua);
        VoxelApi::reg(lua);

        lua.set_function("get_vec3", [](float x, float y, float z) { return ScriptUtils::get_vec3(x,y,z); });
        lua.set_function("get_vec4", [](float x, float y, float z, float a) { return ScriptUtils::get_vec4(x,y,z,a); });
        lua.set_function("get_vec3_val", [](float val) { return ScriptUtils::get_vec3(val); });

        lua.new_usertype<glm::vec3>("vec3",
            sol::constructors<glm::vec3(float), glm::vec3(float, float, float)>(),
            "x", &glm::vec3::x,
            "y", &glm::vec3::y,
            "z", &glm::vec3::z
        );

        lua.new_usertype<glm::vec4>("vec4",
            sol::constructors<glm::vec4(float), glm::vec4(float, float, float, float)>(),
            "r", &glm::vec4::r,
            "g", &glm::vec4::g,
            "b", &glm::vec4::b,
            "a", &glm::vec4::a
        );
        
        lua["physics"] = GameManager::instance().getPhysicsEngine();
        // lua["camera"] = _gm->getRenderer()->getCamera();
    }

    bool loadScript(const std::string& path) {
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

            std::vector<std::string> s = split(path, '.');
            s = split(s[0], '/');
            std::string scriptName = s.back();

            scripts[scriptName] = path;

            Logger::instance().log(LogLevel::SUCCESS, "ScriptSystem", "Script loaded brcyan<", path ,">");

            return true;
        } catch (const sol::error& e) {
            std::cerr << "[Lua Exception] " << e.what() << std::endl;
            return false;
        }
    }

    void startScript(const std::string& scriptName, GameObject* obj) {
        try {
            if (!lua[scriptName].valid()) {
                Logger::instance().log(LogLevel::ERROR, "ScriptSystem", "Script ", scriptName, " not loaded");
                return;
            }
            sol::function startFunc = lua[scriptName]["start"];
            if (startFunc.valid()) {
                startFunc(obj);
            }
        } catch (const sol::error& e) {
            std::cerr << "Lua runtime error: " << e.what() << std::endl;
        }
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
};