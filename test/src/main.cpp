/**
 * @file main.cpp
 * @author Vlad Kartsaev
 * @brief Voxel3D Engine demo application
 * @version 2.0
 * @date 2023-04-06
 *
 * @copyright Copyright (c) 2024 Forever.
 *
 */

#include "voxelx.h"

int WIDTH = 1024;
int HEIGHT = 768;

bool DEBUG_MODE = false;
bool QUIT = false;

float MOUSE_SPEED = 1.3f;

void exit_game() {
    QUIT = true;
}

void debug_mode() {
    DEBUG_MODE = !DEBUG_MODE;
}

void pause_mode() {
    Window::setPause(!Window::getPause());
    Input::toggleCursor();
}

#include <random>
#include "test.h"


std::vector<glm::ivec2> rasterizeLine(glm::ivec2 p1, glm::ivec2 p2) {
    std::vector<glm::ivec2> voxels;

    int x1 = p1.x, y1 = p1.y;
    int x2 = p2.x, y2 = p2.y;

    int dx = abs(x2 - x1), sx = (x1 < x2) ? 1 : -1;
    int dy = -abs(y2 - y1), sy = (y1 < y2) ? 1 : -1;
    int err = dx + dy;

    while (true) {
        voxels.push_back({x1, y1});  // добавляем воксель
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }

    return voxels;
}

void addLineVoxels(const glm::ivec2& a, const glm::ivec2& b, VoxelModel* model, const glm::vec4& color) {
    int x0 = a.x, y0 = a.y;
    int x1 = b.x, y1 = b.y;

    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    while (true) {
        // std::cout << noise.GetNoise((float)x0, (float)y0) << "\n";

        float raw = noise.GetNoise((float)x0 / 5, (float)y0 / 5); // возвращает [-1,1]
        float normalized = (raw + 1.0f) / 2.0f; // теперь [0,1]
        float height = normalized * 100.0f; // теперь [0,100]

        // float amplitude = 100.0f; // максимальная высота
        // float height = ((noise.GetNoise((float)x0, (float)y0) + 1.0f) / 2.0f) * amplitude;

        Voxel voxel;
        voxel.position = glm::vec3(x0, height, y0);
        voxel.color = color;
        model->setVoxel(x0, height, y0, voxel);

        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
        
    }
}

int main() {
    vLogger::setLogLevel(LOGLEVEL::VERBOSE);
  
    std::string _version = _VERSION;
    vLogger::eprint("ENGINE", "VoxelX " + _version + "\n", LOGLEVEL::VERBOSE);

    Window::init(WIDTH, HEIGHT, TITLE);
    Input::init();

    ResourceManager::init("../res/");

    Renderer::init(_RENDERSIZE);
    ResourceManager::loadShaders();
    ResourceManager::loadTextures();
    ResourceManager::loadModels();
    ResourceManager::loadFonts();

    GameManager *gm = new GameManager();

    //! Camera
    Camera *camera = new Camera(glm::vec3(3, 1, 0), glm::radians(70.0f));
    Renderer::addCamera(camera);
    
    MapGenerator mapa;
    std::vector<LinePolygon> test = mapa.makeCellColors(); //drawCellBoundaries();
    
    // Используем для полигона:
    VoxelModel* floorvox = new VoxelModel();

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetFrequency(0.01f); // регулирует масштаб шума
    noise.SetFractalOctaves(3); // детализация
    noise.SetFractalLacunarity(2.0f);
    noise.SetFractalGain(0.1f);


    for (int x = 0; x < 1000; ++x) {
        for (int z = 0; z < 1000; ++z) {
            // шум для высоты
            float nx = x / 1000.0f;
            float nz = z / 1000.0f;
            float n = noise.GetNoise(nx * 1000, nz * 1000); // [-1,1]
            n = (n + 1.0f) / 2.0f; // [0,1]

            int height = int(n * 100);
            height = std::min(height, 100-1);

            // создаём voxel только на нужной высоте
            Voxel voxel;
            voxel.position = glm::ivec3(x, height, z);

            float normalizedHeight = 1.0f - height / 100.0f;
            float l = glm::mix(0.3f, 0.7f, normalizedHeight);
            voxel.color = glm::vec4(mapa.hsl2rgb(normalizedHeight, 0.5f, l), 1.0f);

            floorvox->setVoxel(x, height, z, voxel);
        }
    }
        
    floorvox->setSize(glm::ivec3(1000, 50, 1000));

    // VoxelModel *boxvox = new VoxelModel(glm::ivec3(10, 10, 10));
    // for (int y1 = 0; y1 < boxvox->getSize().y; y1++) {
    //     for (int z1 = 0; z1 < boxvox->getSize().z; z1++) { 
    //         for (int x1 = 0; x1 < boxvox->getSize().x; x1++) {
    //             Voxel *voxel = new Voxel(glm::vec3(x1, y1, z1));
    //             voxel->setColor(glm::vec4(0.2, 0.0f, 0.3f, 1.0f));
    //             boxvox->addVoxel(voxel);
    //         }
    //     }
    // }
    // VoxelModel *lightvox = new VoxelModel(glm::ivec3(5, 5, 5));
    // for (int y1 = 0; y1 < lightvox->getSize().y; y1++) {
    //     for (int z1 = 0; z1 < lightvox->getSize().z; z1++) { 
    //         for (int x1 = 0; x1 < lightvox->getSize().x; x1++) {
    //             Voxel *voxel = new Voxel(glm::vec3(x1, y1, z1));
    //             voxel->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    //             lightvox->addVoxel(voxel);
    //         }
    //     }
    // }

    ResourceManager::addModel(floorvox, "floor");
    // ResourceManager::addModel(boxvox, "box");
    // ResourceManager::addModel(lightvox, "light");

    ResourceManager::prepareModel("floor");
    // ResourceManager::prepareModel("box");
    // ResourceManager::prepareModel("light");

    // ! GameObjects
    GameObject *floorobj = new GameObject(gm, "floor", ResourceManager::getModel("floor"), glm::vec3(-500, -0.1f, -500));
    floorobj->getPhysicsObject()->setMass(1000.0f);
    floorobj->getPhysicsObject()->setType(STATIC_PHYSICS);

    // GameObject *boxobj = new GameObject(gm, "box", ResourceManager::getModel("box"), glm::vec3(-50, 150, 100));
    // boxobj->getPhysicsObject()->setType(DYNAMIC_PHYSICS);

    // GameObject *lightobj = new GameObject(gm, "light", ResourceManager::getModel("light"), glm::vec3(0, 100, -50));
    // lightobj->getPhysicsObject()->setType(KINEMATIC_PHYSICS);

    // auto vox = ResourceManager::getModel("bread");
    // TODO
    // glm::ivec3 size = vox->getSize();
    // size_t count = 0;
    // for (int x = 0; x < size.x; x++) {
    //     for (int y = 0; y < size.y; y++) {
    //         for (int z = 0; z < size.z; z++) {
    //             Voxel* v = vox->getVoxel(glm::ivec3(x,y,z));
    //             if (v) {
    //                 count++;
    //                 std::cout << "Voxel at (" << x << "," << y << "," << z << ") visible=" 
    //                         << v->isVisible() << "\n";
    //             }
    //         }
    //     }
    // }
    // std::cout << "Total voxels: " << count << "\n";
    // std::cout << "Bread model voxels count: " << vox-> ->getVoxelsCount() << "\n";

    GameObject *appleobj = new GameObject(gm, "player", ResourceManager::getModel("apple"), glm::vec3(0, 150, 40));
    appleobj->attachCamera(camera, glm::vec3(0, 15, 0)); //! Attach camera to appleobj, apple hidden
    appleobj->getPhysicsObject()->setType(DYNAMIC_PHYSICS);
    appleobj->getPhysicsObject()->getRigidBody()->setAngularLockAxisFactor(rp3d::Vector3(0, 0, 0));
    rp3d::Vector3 lock = appleobj->getPhysicsObject()->getRigidBody()->getAngularLockAxisFactor();
    GameObject *appleobj1 = new GameObject(gm, "apple", ResourceManager::getModel("apple"), glm::vec3(0, 15, 37));
    appleobj1->getPhysicsObject()->setType(DYNAMIC_PHYSICS);

    GameObject *breadobj = new GameObject(gm, "bread", ResourceManager::getModel("bread"), glm::vec3(50, 30, 20));
    breadobj->getPhysicsObject()->setType(DYNAMIC_PHYSICS);
    // gm->addGameObject(boxobj);
    // gm->addGameObject(lightobj);
    gm->addGameObject(floorobj);
    gm->addGameObject(appleobj);
    gm->addGameObject(appleobj1);
    gm->addGameObject(breadobj);

    float deltaTime = 0.0f;
    auto lastTimePoint = std::chrono::system_clock::now();
    glm::vec2 cam(0.0f, 0.0f);
    float cameraspeed = 100.0f;
    float walkspeed = 25.0f;
    float jumpforce = 500.0f;

    GUI gui;
    FT_Face face = ResourceManager::getFont("arial");
    TextMesh* textMesh = new TextMesh(face);

    Input::add_event_handler(JPRESSED, SDLK_ESCAPE, exit_game);
    Input::add_event_handler(JPRESSED, SDLK_TAB, Input::toggleCursor);
    Input::add_event_handler(JPRESSED, SDLK_F3, debug_mode);
    Input::add_event_handler(JPRESSED, SDLK_F11, Window::toggleFullscreen);
    Input::add_event_handler(JPRESSED, SDLK_p, pause_mode);
    Window::setPause(false);
    Input::toggleCursor();
    float angle = 0.0f;
    Window::sky.x = 0.1f;
    Window::sky.y = 0.1f;
    Window::sky.z = 0.1f;

    while (!QUIT) {
        Input::processEvents(QUIT);
        Input::process_keys();

        auto newTimePoint = std::chrono::system_clock::now();
        auto dtMsec = std::chrono::duration_cast<std::chrono::milliseconds>(newTimePoint - lastTimePoint);
        lastTimePoint = newTimePoint;
        deltaTime = 0.001f * float(dtMsec.count());

        Window::_glClear();

        if (Input::getCursorLock()) {
            cam.x += -Input::deltaX * MOUSE_SPEED / Window::height;
            cam.y += -Input::deltaY * MOUSE_SPEED / Window::height;
            if (cam.y < -glm::radians(89.0f))
                cam.y = -glm::radians(89.0f);
            else if (cam.y > glm::radians(89.0f))
                cam.y = glm::radians(89.0f);

            camera->setRotation(glm::mat4(1.0f));
            camera->rotate(cam, 0);
            // appleobj->setRotation(cam.x, glm::vec3(0, 1, 0));
            // camera->setPosition(appleobj->getPosition() + glm::vec3(0, 3, 0));
        }
        glm::vec3 camera_front = camera->getTarget();
        glm::vec3 camera_up = camera->getUp();
        glm::vec3 camera_right = camera->getRight();

        if (Input::jpressed(SDLK_F3)) {
            if (DEBUG_MODE){
                appleobj->detachCamera();
            } else {
                appleobj->attachCamera(camera);
            }
        }

        if (!Window::getPause()) {
            if (Input::pressed(SDLK_w)) {
                if (DEBUG_MODE)
                    camera->setPosition(camera->getPosition() + glm::vec3(camera_front.x, camera_front.y, camera_front.z) * deltaTime * cameraspeed);
                else {
                    glm::vec3 vel = glm::vec3(camera_front.x, 0, camera_front.z) * deltaTime * walkspeed;
                    appleobj->setPosition(appleobj->getPosition() + vel);
                }
            }
            if (Input::pressed(SDLK_s)) {
                if (DEBUG_MODE)
                    camera->setPosition(camera->getPosition() - glm::vec3(camera_front.x, camera_front.y, camera_front.z) * deltaTime * cameraspeed);
                else {
                    glm::vec3 vel = glm::vec3(camera_front.x, 0, camera_front.z) * deltaTime * walkspeed;
                    appleobj->setPosition(appleobj->getPosition() - vel);
                }
            }
            if (Input::pressed(SDLK_a)) {
                if (DEBUG_MODE)
                    camera->setPosition(camera->getPosition() - glm::vec3(camera_right.x, camera_right.y, camera_right.z) * deltaTime * cameraspeed);
                else {
                    glm::vec3 vel = glm::vec3(camera_right.x, 0, camera_right.z) * deltaTime * walkspeed;
                    appleobj->setPosition(appleobj->getPosition() - vel);
                }
            }
            if (Input::pressed(SDLK_d)) {
                if (DEBUG_MODE)
                    camera->setPosition(camera->getPosition() + glm::vec3(camera_right.x, camera_right.y, camera_right.z) * deltaTime * cameraspeed);
                else {
                    glm::vec3 vel = glm::vec3(camera_right.x, 0, camera_right.z) * deltaTime * walkspeed;
                    appleobj->setPosition(appleobj->getPosition() + vel);
                }
            }

            // if (Input::pressed(SDLK_u)) {
            //     boxobj->setPosition(boxobj->getPosition() + glm::vec3(0, 0, -1) * deltaTime * walkspeed);
            // }
            // if (Input::pressed(SDLK_h)) {
            //     boxobj->setPosition(boxobj->getPosition() + glm::vec3(-1, 0, 0) * deltaTime * walkspeed);
            // }
            // if (Input::pressed(SDLK_k)) {
            //     boxobj->setPosition(boxobj->getPosition() + glm::vec3(1, 0, 0) * deltaTime * walkspeed);
            // }
            // if (Input::pressed(SDLK_j)) {
            //     boxobj->setPosition(boxobj->getPosition() + glm::vec3(0, 0, 1) * deltaTime * walkspeed);
            // }

            if (appleobj->getPhysicsObject()->isGrounded()) {
                if (Input::jpressed(SDLK_SPACE)) {
                    appleobj->getPhysicsObject()->applyForce(glm::vec3(0, jumpforce, 0));
                }
            }

            if (Input::pressed(SDLK_c)) {
                camera->setZoom(0.2f);
            } else {
                camera->setZoom(1.0f);
            }

            if (Input::jclicked(SDL_BUTTON_MIDDLE)) {
                appleobj1->getPhysicsObject()->applyForce(glm::vec3(0, jumpforce * 5, -jumpforce * 5));
            }
            if (Input::jclicked(SDL_BUTTON_RIGHT)) {
                appleobj->setPosition(glm::vec3(0, 100, 0));
            }
 
            // angle += 0.01f;
            // float radius = 100.0f;

            // lightobj->setPosition(glm::vec3(radius * cos(angle), 50, radius * sin(angle)));
        }
        
        // int test = (int)appleobj->getPhysicsObject()->isGrounded();
        

        Light light;
        light.position = glm::vec3(0.0f); //lightobj->getPosition();
        light.direction = camera->getTarget();
        light.cutOff = glm::cos(glm::radians(15.0f));
        light.outerCutOff = glm::cos(glm::radians(30.0f));
        
        light.ambient = glm::vec3(0.1f);
        light.diffuse = glm::vec3(0.8f);
        light.specular = glm::vec3(1.0f);
        light.constant = 0.1f;
        light.linear = 0.014f;
        light.quadratic = 0.00007f;

        ResourceManager::getShader("voxel")->use();        
        ResourceManager::getShader("voxel")->uniformMatrix("projection", Renderer::getCamera()->getProjection());
        ResourceManager::getShader("voxel")->uniformMatrix("view", Renderer::getCamera()->getView());

        gm->UpdatePhysics(deltaTime);
        gm->Update(light);

        //? TEXT
        ResourceManager::getShader("font")->use();
        ResourceManager::getShader("font")->uniformMatrix("projection", glm::ortho(0.0f, (float)Window::width, 0.0f, (float)Window::height));
        ResourceManager::getShader("font")->uniformVec3("textColor", glm::vec3(0.9f, 0.3f, 0.9f));
        textMesh->draw("VoxelX: " + _version, 5.0f, (float)Window::height-20.0f, 0.4f);

        if (DEBUG_MODE) {
            ResourceManager::getShader("font")->uniformVec3("textColor", glm::vec3(0.1f, 0.7f, 0.1f));

            textMesh->draw("Debug mode", Window::width / 2 - 80.0f, Window::height-20.0f, 0.5f);
            textMesh->draw("FPS: " + std::to_string((int)(1.0f / deltaTime)), 5.0f, Window::height-50.0f, 0.4f);
            textMesh->draw("GameObjects: " + std::to_string(gm->getGameObjectsSize()), 5.0f, Window::height-70.0f, 0.4f);

            textMesh->draw("Camera position: " + std::to_string(camera->getPosition().x)    + " " + std::to_string(camera->getPosition().y)    + " " + std::to_string(camera->getPosition().z), 5.0f,    Window::height - 115.0f, 0.4f);
            textMesh->draw("Player position: " + std::to_string(appleobj->getPosition().x)  + " " + std::to_string(appleobj->getPosition().y)  + " " + std::to_string(appleobj->getPosition().z), 5.0f,  Window::height - 135.0f, 0.4f);
            textMesh->draw("Camera Front: "    + std::to_string(camera->getTarget().x) + " " + std::to_string(camera->getTarget().y) + " " + std::to_string(camera->getTarget().z), 5.0f, Window::height - 155.0f, 0.4f);
            // textMesh->draw("LIGHT Object position: " + std::to_string(lightobj->getPosition().x)  + " " + std::to_string(lightobj->getPosition().y)  + " " + std::to_string(lightobj->getPosition().z), 5.0f,  Window::height - 175.0f, 0.4f);
            // textMesh->draw("BOX Object position: " + std::to_string(boxobj->getPosition().x)  + " " + std::to_string(boxobj->getPosition().y)  + " " + std::to_string(boxobj->getPosition().z), 5.0f,  Window::height - 195.0f, 0.4f);
        
            ResourceManager::getShader("octoline")->use();
            ResourceManager::getShader("octoline")->uniformMatrix("projviewmodel", Renderer::getCamera()->getProjection() * Renderer::getCamera()->getView() * glm::mat4(1.0f));
            rp3d::DebugRenderer& dbgrndr = gm->getPhysicsEngine()->getPhysicsWorld()->getDebugRenderer();

            ResourceManager::getShader("octoline")->uniformVec3("clr", glm::vec3(0.2f, 1.0f, 0.2f));
            if (dbgrndr.getNbLines() != 0) {
                float lines[dbgrndr.getNbLines() * 2 * 3];
                int i = 0;
                for (rp3d::DebugRenderer::DebugLine line : dbgrndr.getLines()) {
                    lines[i + 0] = line.point1.x;
                    lines[i + 1] = line.point1.y;
                    lines[i + 2] = line.point1.z;

                    lines[i + 3] = line.point2.x;
                    lines[i + 4] = line.point2.y;
                    lines[i + 5] = line.point2.z;
                    i += 6;
                }
                int attrs[2] = { 3,  0 };
                Mesh* _mesh = new Mesh(lines, dbgrndr.getNbLines(), attrs);
                _mesh->draw(GL_LINES);
                delete _mesh;
            }

            ResourceManager::getShader("octoline")->uniformVec3("clr", glm::vec3(1.0f, 0.2f, 0.0f));
            if (dbgrndr.getNbTriangles() != 0) {
                float triangles[dbgrndr.getNbTriangles() * 3 * 3];
                int i = 0;
                for (rp3d::DebugRenderer::DebugTriangle trian : dbgrndr.getTriangles()) {
                    triangles[i + 0] = trian.point1.x;
                    triangles[i + 1] = trian.point1.y;
                    triangles[i + 2] = trian.point1.z;

                    triangles[i + 3] = trian.point2.x;
                    triangles[i + 4] = trian.point2.y;
                    triangles[i + 5] = trian.point2.z;

                    triangles[i + 6] = trian.point3.x;
                    triangles[i + 7] = trian.point3.y;
                    triangles[i + 8] = trian.point3.z;

                    i += 9;
                }
                int attrs[2] = { 3,  0 };
                Mesh* _mesh = new Mesh(triangles, dbgrndr.getNbTriangles() * 3, attrs);
                _mesh->draw(GL_LINES);
                delete _mesh;
            }
        }
        gui.draw();

        Window::swapBuffers();
        Input::pullEvents();
    }
    std::cout << "\n";
    ResourceManager::cleanup();
    Input::cleanup();
    Window::exit();
    return 0;
}