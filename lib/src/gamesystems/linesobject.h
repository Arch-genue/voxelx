#pragma once

#include <glm/glm.hpp>

#include "../graphics/renderer.h"
#include "transformobject.h"
#include "modelobject.h"
#include "../loaders/resourcemanager.h"
#include "../graphics/renderer.h"

class Mesh;
class Shader;
class Renderer;
class GameManager;

class LinesObject : public TransformObject, public ModelObject {
private:
    Mesh* _mesh;

    //? GameManager global object
    GameManager* _gm;

public:
    LinesObject(float* vertices, uint count);

    void updateMesh(float* vertices, uint count);

    void draw();
};