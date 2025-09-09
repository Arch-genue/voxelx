#pragma once

class GameObjectApi {
public:
    static void reg (sol::state& lua) {
        // lua.new_usertype<GameObject>("GameObject",
        //     "setPosition", &GameObject::setPosition,
        //     "getPosition", &GameObject::getPosition,
        //     "getName", &GameObject::getName,
        //     "getID", &GameObject::getID,
        //     "getPhysicsObject", &GameObject::getPhysicsObject
        // );
    }
};