#include "voxelx.h"
// #include "gamesystems/mapgenerator.h"
#include "assetmanager/assetmanager.h"
#include "gamesystems/camera.h"
// #include "physics/physicsengine.h"
// #include "scripting/scriptsystem.hpp"
// #include "debugui.hpp"

#include "ecs/ecs.h"
#include "utilities/logger.hpp"

#include <iostream>

bool QUIT = false;

void exit_game() { QUIT = true; }

int main() {
    Logger::instance().setLogLevel(LogLevel::VERBOSE);
  
    Window window(1024, 768, "VoxelX Demo");
    Input::instance().setWindow(&window);

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

    // Voxel vox;
    // vox.setColor(glm::vec4(0.4f));
    // vox.mat = 0x2;
    // vox.visible = true;
    // std::cout << "Voxel size: " << sizeof(vox) << "\n";

    // VoxelChunk chunk;
    // std::cout << "Chunk size: " << sizeof(chunk) << "\n";

    //* 
    // MapGenerator map(glm::ivec3(500, 200, 500));
    // VoxelStructure* voxelmodel = map.generateMap();

    // AssetManager::instance().addModel(voxelmodel, "map");
    // AssetManager::instance().prepareStructure("map");

    glm::vec2 cam(0.0f, 0.0f);
    float cameraspeed = 100.0f;

    float MOUSE_SPEED = 1.3f;

    //! GameObjects
    // GameObject *wallobj = new GameObject("wallobj", AssetManager::instance().getMeshFromModel("wall"), glm::vec3(60, 300, 0));
    // wallobj->getPhysicsObject()->setRigidBody(gm.getPhysicsEngine()->createRigidBody(wallobj->getPosition()));
    // wallobj->getPhysicsObject()->setPhysicsType(STATIC_PHYSICS);
    // wallobj->getPhysicsObject()->setCollider(gm.getPhysicsEngine()->getPhysicsCommon().createBoxShape(rp3d::Vector3(1.5f, 20, 50)));

    // GameObject *mapobj = new GameObject("mapobj", AssetManager::instance().clone<GameModel>("map"), glm::vec3(0, 0, 0));
    // mapobj->getPhysicsObject()->setRigidBody(gm.getPhysicsEngine()->createRigidBody(mapobj->getPosition()));
    // mapobj->getPhysicsObject()->setPhysicsType(STATIC_PHYSICS);
    // mapobj->getPhysicsObject()->setCollider(gm.getPhysicsEngine()->getPhysicsCommon().createBoxShape(rp3d::Vector3(300, 100, 300)));

    // GameObject *appleobj = new GameObject("appleobj", AssetManager::instance().getMeshFromModel("apple"), glm::vec3(0, 200, 0));
    // appleobj->getPhysicsObject()->setRigidBody(gm.getPhysicsEngine()->createRigidBody(appleobj->getPosition()));
    // appleobj->getPhysicsObject()->setPhysicsType(DYNAMIC_PHYSICS);
    // appleobj->getPhysicsObject()->setCollider(gm.getPhysicsEngine()->getPhysicsCommon().createSphereShape(10));
    // appleobj->getPhysicsObject()->getRigidBody()->setAngularLockAxisFactor(rp3d::Vector3(0, 0, 0));
    // appleobj->attachScript("test");
    // appleobj->attachCamera(camera);

    // gm.addGameObject(mapobj);
    // gm.addGameObject(appleobj);
    // gm.addGameObject(wallobj);
    // gm.addGameObject(wall1obj);

    float jumpforce = 500.0f;

    float currentTime = 0.0f;
    float deltaTime = 0.0f;
    float lastTime = 0.0f;

    //!!

    ECSManager ecsmanager;
    PhysicsSystem& physicssystem = ecsmanager.physicsSystem;

    Entity player = ecsmanager.createEntity();
    ecsmanager.addComponent<Transform>(player, glm::vec3(1,2,3), glm::vec3(0,0,0), glm::vec3(1,1,1));
    ecsmanager.addComponent<RigidBody>(player, ecsmanager.physicsSystem.getWorld(), rp3d::Transform().identity());
    ecsmanager.getComponent<RigidBody>(player).body->setType(rp3d::BodyType::DYNAMIC);
    ecsmanager.addComponent<Collider>(player, ecsmanager.getComponent<RigidBody>(player).body, physicssystem.physicsCommon.createBoxShape(rp3d::Vector3(1, 1, 1)), rp3d::Transform().identity());
    ecsmanager.addComponent<Render>(player, "apple");

    // rp3d::Transform tran = ecsmanager.getComponent<RigidBody>(player).body->getTransform();
    // tran.setPosition(rp3d::Vector3(50, 0, 100));
    // ecsmanager.getComponent<RigidBody>(player).body->setTransform(tran);
    
    // auto& t = ecsmanager.getComponent<Transform>(player);
    // std::cout << "Matrix[3]: "
    //           << t.modelMatrix[3].x << " "
    //           << t.modelMatrix[3].y << " "
    //           << t.modelMatrix[3].z << "\n";

    while (!QUIT) {
        Input::instance().processEvents(QUIT);
        Input::instance().process_keys();

        // currentTime = SDL_GetTicks() / 1000.0f;
        // deltaTime = currentTime - lastTime;
        // lastTime = currentTime;
        
        window._glClear();

        // rp3d::Vector3 rpos = ecsmanager.getComponent<RigidBody>(player).body->getTransform().getPosition();
        // Logger::instance().log(LogLevel::DEBUG, "Rigidbody", "Position: ", rpos.x, " ", rpos.y, " ", rpos.z);

        // glm::vec3 dpos = ecsmanager.getComponent<Transform>(player).position;
        // Logger::instance().log(LogLevel::DEBUG, "Transform", "Position: ", dpos.x, " ", dpos.y, " ", dpos.z);
        
        AssetManager::instance().get<Shader>("voxel")->use();
        AssetManager::instance().get<Shader>("voxel")->uniformMatrix("projection", camera->getProjection((float)window.getWidth() / (float)window.getHeight()));
        AssetManager::instance().get<Shader>("voxel")->uniformMatrix("view", camera->getView());
        AssetManager::instance().get<Shader>("voxel")->uniformVec3("viewPos", camera->getPosition());
        ecsmanager.update();

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

        // window.startFrame();

        // ShowDebug();
        // if (window.getPause()) {        
        //     static int test = 0;

        //     ShowInspector(gm.getGameObjects(), test);
        //     DebugPhysics((float)window.getWidth() / (float)window.getHeight());
        // }

        // window.renderGUI();

        window.swapBuffers();
        Input::instance().pullEvents();
    }

    return 0;
}