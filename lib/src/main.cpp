#include "voxelx.h"
// #include "gamesystems/mapgenerator.h"
#include "assetmanager/assetmanager.h"
#include "gamesystems/camera.h"
// #include "scripting/scriptsystem.hpp"
#include "debugui.hpp"

#include "ecs/ecs.h"
#include "utilities/logger.hpp"

#include <iostream>
#include <chrono>

bool QUIT = false;

void exit_game() { QUIT = true; }

#include <random>

std::mt19937 rng(std::random_device{}());

int main() {
    Logger::instance().setLogLevel(LogLevel::VERBOSE);
  
    Window window(1024, 768, "VoxelX Demo");
    Input::instance().setWindow(&window);
    Renderer::reserve();
    // Renderer::initInstancing();

    AssetManager::instance().init("/home/user/dev/voxelx/build/res/");
    AssetManager::instance().loadShaders();
    AssetManager::instance().loadGameModels();
    // AssetManager::instance().loadScripts();

    // GameManager& gm = GameManager::instance();

    //! Camera
    Camera *camera = new Camera(glm::vec3(3, 20, 0), glm::radians(70.0f));
    // gm.setCamera(camera);
    
    Input::instance().add_event_handler(JPRESSED, SDLK_ESCAPE, exit_game);
    Input::instance().add_event_handler(JPRESSED, SDLK_TAB, [](){ Input::instance().toggleCursor(); });
    Input::instance().add_event_handler(JPRESSED, SDLK_F11, [&](){ window.toggleFullscreen(); });
    Input::instance().add_event_handler(JPRESSED, SDLK_p, [&](){ window.setPause(!window.getPause()); });
    window.setPause(false);

    window.setSky({0.1f, 0.1f, 0.1f});

    //* 
    // MapGenerator map(glm::ivec3(500, 200, 500));
    // VoxelStructure* voxelmodel = map.generateMap();

    // AssetManager::instance().addModel(voxelmodel, "map");
    // AssetManager::instance().prepareStructure("map");

    glm::vec2 cam(0.0f, 0.0f);
    float cameraspeed = 100.0f;

    float MOUSE_SPEED = 1.3f;

    float jumpforce = 500.0f;

    //!!

    ECSManager ecsmanager;
    PhysicsSystem& physicssystem = ecsmanager.physicsSystem;

    Entity map = ecsmanager.createEntity();
    glm::vec3 mapposition(0, -600, 0);
    ecsmanager.addComponent<Name>(map, "map");
    ecsmanager.addComponent<Transform>(map, mapposition, glm::vec3(0,0,0), glm::vec3(1,1,1));
    ecsmanager.addComponent<RigidBody>(map, ecsmanager.physicsSystem.getWorld(), mapposition, rp3d::BodyType::STATIC);
    ecsmanager.addComponent<Collider>(map, ecsmanager.getComponent<RigidBody>(map).body, physicssystem.physicsCommon.createBoxShape(rp3d::Vector3(500, 150, 500)), rp3d::Transform().identity());
    ecsmanager.addComponent<Render>(map, "map");

    Entity player = ecsmanager.createEntity();
    glm::vec3 plposition(1, 300, -50);
    ecsmanager.addComponent<Name>(player, "Apple");
    ecsmanager.addComponent<Transform>(player, plposition, glm::vec3(0,0,0), glm::vec3(1,1,1));
    ecsmanager.addComponent<RigidBody>(player, ecsmanager.physicsSystem.getWorld(), plposition, rp3d::BodyType::STATIC);
    rp3d::Transform trt;
    trt.setPosition(rp3d::Vector3(0, 1, 2));
    ecsmanager.addComponent<Collider>(player, ecsmanager.getComponent<RigidBody>(player).body, physicssystem.physicsCommon.createBoxShape(rp3d::Vector3(5, 5, 5)), trt);
    ecsmanager.addComponent<Render>(player, "apple");

    Entity player1 = ecsmanager.createEntity();
    glm::vec3 pl1position(1, 200, 50);
    ecsmanager.addComponent<Transform>(player1, pl1position, glm::vec3(0,0,0), glm::vec3(1,1,1));
    ecsmanager.addComponent<RigidBody>(player1, ecsmanager.physicsSystem.getWorld(), pl1position, rp3d::BodyType::STATIC);
    ecsmanager.addComponent<Collider>(player1, ecsmanager.getComponent<RigidBody>(player1).body, physicssystem.physicsCommon.createBoxShape(rp3d::Vector3(5, 5, 5)), rp3d::Transform().identity());
    ecsmanager.addComponent<Render>(player1, "apple");

    ecsmanager.serialize(player);

    std::uniform_real_distribution<float> offsetDist(-5.0f, 5.0f); // небольшой разброс
    std::uniform_real_distribution<float> rotDist(0.0f, glm::pi<float>() * 2);

    int rows = 15;
    int cols = 15;

    // for (int i = 0; i < rows; i++) {
    //     for (int j = 0; j < cols; j++) {
    //         Entity obj = ecsmanager.createEntity();

    //         // Плотнее ставим объекты, шаг ~размер яблока
    //         glm::vec3 objposition(
    //             20 + j * 5 + offsetDist(rng),  // шаг 12 вместо 30
    //             200 + offsetDist(rng),          // небольшое смещение по высоте
    //             50 - i * 5 + offsetDist(rng)   // шаг 12
    //         );

    //         glm::vec3 randomRotation(rotDist(rng), rotDist(rng), rotDist(rng));

    //         ecsmanager.addComponent<Transform>(obj, objposition, randomRotation, glm::vec3(1,1,1));
    //         ecsmanager.addComponent<RigidBody>(obj, ecsmanager.physicsSystem.getWorld(), objposition, rp3d::BodyType::DYNAMIC);
    //         ecsmanager.addComponent<Collider>(
    //             obj, 
    //             ecsmanager.getComponent<RigidBody>(obj).body, 
    //             physicssystem.physicsCommon.createBoxShape(rp3d::Vector3(5, 5, 5)), 
    //             rp3d::Transform().identity()
    //         );
    //         auto& collider = ecsmanager.getComponent<Collider>(obj);
    //         collider.collider->getMaterial().setBounciness(0.6f); // от 0 до 1
    //         collider.collider->getMaterial().setFrictionCoefficient(0.3f);
    //         ecsmanager.addComponent<Render>(obj, "apple");
    //     }
    // }

    // rp3d::Transform tran = ecsmanager.getComponent<RigidBody>(player).body->getTransform();
    // tran.setPosition(rp3d::Vector3(50, 0, 100));
    // ecsmanager.getComponent<RigidBody>(player).body->setTransform(tran);
    
    // auto& t = ecsmanager.getComponent<Transform>(player);
    // std::cout << "Matrix[3]: "
    //           << t.modelMatrix[3].x << " "
    //           << t.modelMatrix[3].y << " "
    //           << t.modelMatrix[3].z << "\n";

    using Clock = std::chrono::high_resolution_clock;
    auto lastTime = Clock::now();

    while (!QUIT) {
        Input::instance().processEvents(QUIT);
        Input::instance().process_keys();

        auto currentTime = Clock::now();
        std::chrono::duration<float> frameTime = currentTime - lastTime;
        float deltaTime = frameTime.count(); // в секундах
        lastTime = currentTime;
        
        window._glClear();
        
        AssetManager::instance().get<Shader>("voxel")->use();
        AssetManager::instance().get<Shader>("voxel")->uniformMatrix("projection", camera->getProjection((float)window.getWidth() / (float)window.getHeight()));
        AssetManager::instance().get<Shader>("voxel")->uniformMatrix("view", camera->getView());
        AssetManager::instance().get<Shader>("voxel")->uniformVec3("viewPos", camera->getPosition());
        ecsmanager.update(deltaTime);

        if (Input::instance().getCursorLock()) {
            cam.x += -Input::instance().get_delta_x() * MOUSE_SPEED / window.getHeight();
            cam.y += -Input::instance().get_delta_y() * MOUSE_SPEED / window.getHeight();
            
            cam.y = glm::clamp(cam.y, -glm::radians(89.0f), glm::radians(89.0f));

            camera->setRotation(glm::mat4(1.0f));
            camera->rotate(cam, 0);
        }

        glm::vec3 camera_front = camera->getTarget();
        glm::vec3 camera_up = camera->getUp();
        glm::vec3 camera_right = camera->getRight();

        // if (Input::instance().jpressed(SDLK_F3)) {
        //     if (appleobj->getCamera()){
        //         appleobj->detachCamera();
        //     } else {
        //         appleobj->attachCamera(camera);
        //     }
        // }

        if (!window.getPause()) {
            if (Input::instance().pressed(SDLK_w)) {
                camera->setPosition(camera->getPosition() + glm::vec3(camera_front.x, camera_front.y, camera_front.z) * deltaTime * cameraspeed);
            }
            if (Input::instance().pressed(SDLK_s)) {
                camera->setPosition(camera->getPosition() - glm::vec3(camera_front.x, camera_front.y, camera_front.z) * deltaTime * cameraspeed);
            }
            if (Input::instance().pressed(SDLK_a)) {
                camera->setPosition(camera->getPosition() - glm::vec3(camera_right.x, camera_right.y, camera_right.z) * deltaTime * cameraspeed);
            }
            if (Input::instance().pressed(SDLK_d)) {
                camera->setPosition(camera->getPosition() + glm::vec3(camera_right.x, camera_right.y, camera_right.z) * deltaTime * cameraspeed);
            }

            if (Input::instance().pressed(SDLK_c)) {
                camera->setZoom(0.2f);
            } else {
                camera->setZoom(1.0f);
            }

            if (Input::instance().jclicked(SDL_BUTTON_MIDDLE)) {
                // appleobj->getPhysicsObject()->applyForce(glm::vec3(0, jumpforce * 5, -jumpforce * 5));
            }
        }

        if (Input::instance().jclicked(SDL_BUTTON_MIDDLE)) {
            ecsmanager.removeComponent<Render>(player);
            ecsmanager.destroy(player);
            
            for (int i = 0; i < rows; i++) {
                continue;
                for (int j = 0; j < cols; j++) {
                    Entity obj = ecsmanager.createEntity();

                    // Плотнее ставим объекты, шаг ~размер яблока
                    glm::vec3 objposition(
                        20 + j * 5 + offsetDist(rng),  // шаг 12 вместо 30
                        200 + offsetDist(rng),          // небольшое смещение по высоте
                        50 - i * 5 + offsetDist(rng)   // шаг 12
                    );

                    glm::vec3 randomRotation(rotDist(rng), rotDist(rng), rotDist(rng));

                    ecsmanager.addComponent<Transform>(obj, objposition, randomRotation, glm::vec3(1,1,1));
                    ecsmanager.addComponent<RigidBody>(obj, ecsmanager.physicsSystem.getWorld(), objposition, rp3d::BodyType::DYNAMIC);
                    ecsmanager.addComponent<Collider>(
                        obj, 
                        ecsmanager.getComponent<RigidBody>(obj).body, 
                        physicssystem.physicsCommon.createBoxShape(rp3d::Vector3(5, 5, 5)), 
                        rp3d::Transform().identity()
                    );
                    auto& collider = ecsmanager.getComponent<Collider>(obj);
                    collider.collider->getMaterial().setBounciness(0.6f); // от 0 до 1
                    collider.collider->getMaterial().setFrictionCoefficient(0.3f);
                    ecsmanager.addComponent<Render>(obj, "apple");
                }
            }
        }
        // Light light;
        // light.position = camera->getPosition(); //glm::vec3(0.0f, 150.0f, 0.0f); //lightobj->getPosition();
        // light.direction = camera->getTarget();
        // light.cutOff = glm::cos(glm::radians(10.0f));
        // light.outerCutOff = glm::cos(glm::radians(15.0f));
        
        // light.ambient = glm::vec3(0.2f); // слабый фоновый свет
        // light.diffuse = glm::vec3(1.0f, 1.0f, 1.0f); // яркость основного света
        // light.specular = glm::vec3(1.0f); // отражённые блики
        // light.constant = 1.0f;
        // light.linear = 0.000014f;
        // light.quadratic = 0.0000032f;

        // Camera* camera = gm.getCamera();

        // AssetManager::instance().get<Shader>("voxel")->use();        
        // AssetManager::instance().get<Shader>("voxel")->uniformMatrix("projection", camera->getProjection((float)window.getWidth() / (float)window.getHeight()));
        // AssetManager::instance().get<Shader>("voxel")->uniformMatrix("view", camera->getView());
        // AssetManager::instance().get<Shader>("voxel")->uniformVec3("viewPos", camera->getPosition());

        // gm.UpdatePhysics(deltaTime);
        // gm.Update(light);

        window.startFrame();

        ShowDebug();
        if (window.getPause()) {        
            static int test = 0;

            ShowInspector(ecsmanager, test);
            // DebugPhysics((float)window.getWidth() / (float)window.getHeight());
        }

        window.renderGUI();

        window.swapBuffers();
        Input::instance().pullEvents();
    }

    return 0;
}