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

#include <signal.h>
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

void posix_death_signal(int signum) {
    signal(signum, SIG_DFL); // перепосылка сигнала
	exit(3); //выход из программы. Если не сделать этого, то обработчик будет вызываться бесконечно.
}

int main() {
    signal(SIGSEGV, posix_death_signal);

    Logger::instance().setLogLevel(LogLevel::VERBOSE);
  
    std::string _version = _VERSION;
    Logger::instance().log("ENGINE", "VoxelX " + _version + "\n", LogLevel::VERBOSE);

    Window::init(WIDTH, HEIGHT, TITLE);
    Input::init();

    ResourceManager::init("../res/");

    Renderer::init();
    ResourceManager::loadShaders();
    ResourceManager::loadTextures();
    ResourceManager::loadModels();
    ResourceManager::loadFonts();

    GameManager *gm = new GameManager();

    //! Camera
    Camera *camera = new Camera(glm::vec3(3, 1, 0), glm::radians(70.0f));
    Renderer::addCamera(camera);
    
    MapGenerator mapgen(glm::vec3(1000, 100, 1000));
    VoxelModel* mapmodel = mapgen.generateMap();
    mapmodel->setForbiddenSide("bottom", false);

    ResourceManager::addModel(mapmodel, "floor");
    ResourceManager::prepareModel("floor");

    //! GameObjects
    GameObject *floorobj = new GameObject(gm, "floor", ResourceManager::getModel("floor"), glm::vec3(0, 0, 0));
    floorobj->getPhysicsObject()->setPhysicsType(STATIC_PHYSICS);

    GameObject *appleobj = new GameObject(gm, "player", ResourceManager::getModel("apple"), glm::vec3(0, 150, 40));
    appleobj->attachCamera(camera, glm::vec3(0, 15, 0)); //! Attach camera to appleobj, apple hidden
    appleobj->getPhysicsObject()->setPhysicsType(DYNAMIC_PHYSICS);
    appleobj->getPhysicsObject()->getRigidBody()->setAngularLockAxisFactor(rp3d::Vector3(0, 0, 0));

    GameObject *appleobj1 = new GameObject(gm, "apple", ResourceManager::getModel("apple"), glm::vec3(0, 15, 37));
    appleobj1->getPhysicsObject()->setPhysicsType(DYNAMIC_PHYSICS);

    GameObject *breadobj = new GameObject(gm, "bread", ResourceManager::getModel("bread"), glm::vec3(50, 30, 20));
    breadobj->getPhysicsObject()->setPhysicsType(DYNAMIC_PHYSICS);

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

    // GUI gui;
    FT_Face face = ResourceManager::getFont("arial");
    TextMesh* textMesh = new TextMesh(face);

    Input::add_event_handler(JPRESSED, SDLK_ESCAPE, exit_game);
    Input::add_event_handler(JPRESSED, SDLK_TAB, Input::toggleCursor);
    Input::add_event_handler(JPRESSED, SDLK_F3, debug_mode);
    Input::add_event_handler(JPRESSED, SDLK_F11, Window::toggleFullscreen);
    Input::add_event_handler(JPRESSED, SDLK_p, pause_mode);
    Window::setPause(false);
    Input::toggleCursor();

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

        Light light;
        light.position = camera->getPosition(); //glm::vec3(0.0f, 150.0f, 0.0f); //lightobj->getPosition();
        light.direction = camera->getTarget();
        light.cutOff = glm::cos(glm::radians(10.0f));
        light.outerCutOff = glm::cos(glm::radians(15.0f));
        
        light.ambient = glm::vec3(0.2f); // слабый фоновый свет
        light.diffuse = glm::vec3(1.0f, 1.0f, 1.0f); // яркость основного света
        light.specular = glm::vec3(1.0f); // отражённые блики
        light.constant = 1.0f;
        light.linear = 0.000014f;
        light.quadratic = 0.0000032f;

        ResourceManager::getShader("voxel")->use();        
        ResourceManager::getShader("voxel")->uniformMatrix("projection", Renderer::getCamera()->getProjection());
        ResourceManager::getShader("voxel")->uniformMatrix("view", Renderer::getCamera()->getView());
        ResourceManager::getShader("voxel")->uniformVec3("viewPos", Renderer::getCamera()->getPosition());

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
                std::vector<float> lines;
                lines.reserve(dbgrndr.getNbLines() * 2 * 3);
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
                MeshModel* _mesh = new MeshModel(lines, dbgrndr.getNbLines(), attrs);
                _mesh->draw(GL_LINES);
                delete _mesh;
            }

            ResourceManager::getShader("octoline")->uniformVec3("clr", glm::vec3(1.0f, 0.2f, 0.0f));
            if (dbgrndr.getNbTriangles() != 0) {
                std::vector<float> triangles;
                triangles.reserve(dbgrndr.getNbTriangles() * 3 * 3);
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
                MeshModel* _mesh = new MeshModel(triangles, dbgrndr.getNbTriangles() * 3, attrs);
                _mesh->draw(GL_LINES);
                delete _mesh;
            }
        }
        // gui.draw();

        Window::swapBuffers();
        Input::pullEvents();
    }
    std::cout << "\n";
    ResourceManager::cleanup();
    Input::cleanup();
    Window::exit();
    return 0;
}