#include "voxelx.h"
#include "gamesystems/mapgenerator.h"
#include "loaders/resourcemanager.h"
#include "physics/physicsengine.h"
#include "scripting/scriptsystem.hpp"

#include "imgui.h"

bool QUIT = false;

void exit_game() {
    QUIT = true;
}

int main() {
    Logger::instance().setLogLevel(LogLevel::VERBOSE);
  
    Window::init(1024, 768, "VoxelX Demo");
    Input::init();

    ResourceManager::init("../res/");
    ResourceManager::loadShaders();
    ResourceManager::loadModels();

    ResourceManager::loadScripts();

    GameManager& gm = GameManager::instance();

    //! Camera
    Camera *camera = new Camera(glm::vec3(3, 20, 0), glm::radians(70.0f));
    Renderer::addCamera(camera);
    
    Input::add_event_handler(JPRESSED, SDLK_ESCAPE, exit_game);
    Input::add_event_handler(JPRESSED, SDLK_TAB, Input::toggleCursor);
    Input::add_event_handler(JPRESSED, SDLK_F11, Window::toggleFullscreen);
    Window::setPause(false);

    Window::sky.x = 0.1f;
    Window::sky.y = 0.1f;
    Window::sky.z = 0.1f;

    Voxel vox;
    vox.setColor(glm::vec4(0.4f));
    vox.mat = 0x2;
    vox.visible = true;
    std::cout << "Voxel size: " << sizeof(vox) << "\n";

    Chunk<16> chunk;
    std::cout << "Chunk size: " << sizeof(chunk) << "\n";

    //* 
    MapGenerator map(glm::ivec3(1000, 200, 1000));
    VoxelModel* voxelmodel = map.generateMap();

    ResourceManager::addModel(voxelmodel, "map");
    ResourceManager::prepareModel("map");

    glm::vec2 cam(0.0f, 0.0f);
    float cameraspeed = 100.0f;

    float MOUSE_SPEED = 1.3f;

    //! GameObjects
    GameObject *mapobj = new GameObject("mapobj", ResourceManager::getMeshFromModel("map"), glm::vec3(0, 0, 0));
    mapobj->getPhysicsObject()->setRigidBody(std::make_unique<RigidBodyComponent>(gm.getPhysicsEngine()->createRigidBody(mapobj->getPosition())));
    mapobj->getPhysicsObject()->setPhysicsType(STATIC_PHYSICS);
    mapobj->getPhysicsObject()->setCollider(std::make_unique<ColliderComponent>(gm.getPhysicsEngine()->getPhysicsCommon().createBoxShape(rp3d::Vector3(300, 200, 300))));

    int minX = -250;
    int maxX = 250;
    int minZ = -250;
    int maxZ = 250;

    int width = maxX - minX;
    int length = maxZ - minZ;

    // std::vector<rp3d::Message> messages;

    // rp3d::HeightField* heightField = gm.getPhysicsEngine()->getPhysicsCommon().createHeightField(
    //     width, length, map._heightarray.data(),
    //     rp3d::HeightField::HeightDataType::HEIGHT_INT_TYPE,
    //     messages,
    //     1.0f // integerHeightScale, если "1 int = 1 юнит"
    // );

    // // Display the messages (info, warning and errors)
    // if (messages.size() > 0) {
    
    //     for (const rp3d::Message& message: messages) {
    
    //         std::string messageType;
    
    //         switch(message.type) {
    //             case rp3d::Message::Type::Information:
    //                 messageType = "info";
    //                 break;
    //             case rp3d::Message::Type::Warning:
    //                 messageType = "warning";
    //                 break;
    //             case rp3d::Message::Type::Error:
    //                 messageType = "error";
    //                 break;
    //         }
    
    //         std::cout << "Message (" << messageType << "): " << message.text << std::endl;
    //     }
    // }

    // rp3d::Vector3 scaling(1.0f, 1.0f, 1.0f); // масштабирование по XYZ
    // rp3d::HeightFieldShape* heightShape = gm.getPhysicsEngine()->getPhysicsCommon().createHeightFieldShape(heightField);

    // mapobj->getPhysicsObject()->setCollider(std::make_unique<ColliderComponent>(heightShape));

    GameObject *appleobj = new GameObject("appleobj", ResourceManager::getMeshFromModel("apple"), glm::vec3(0, 500, 0));
    appleobj->getPhysicsObject()->setRigidBody(std::make_unique<RigidBodyComponent>(gm.getPhysicsEngine()->createRigidBody(appleobj->getPosition())));
    appleobj->getPhysicsObject()->setPhysicsType(DYNAMIC_PHYSICS);
    appleobj->getPhysicsObject()->setCollider(std::make_unique<ColliderComponent>(gm.getPhysicsEngine()->getPhysicsCommon().createSphereShape(10)));
    appleobj->attachScript("test");
    // appleobj->getPhysicsObject()->getRigidBody()->setAngularLockAxisFactor(glm::ivec3(1, 1, 1));
    // appleobj->attachCamera(camera);

    gm.addGameObject(mapobj);
    gm.addGameObject(appleobj);

    float jumpforce = 500.0f;

    float currentTime = 0.0f;
    float deltaTime = 0.0f;
    float lastTime = 0.0f;
    while (!QUIT) {
        Input::processEvents(QUIT);
        Input::process_keys();

        currentTime = SDL_GetTicks() / 1000.0f;
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
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
            if (appleobj->getCamera()){
                appleobj->detachCamera();
            } else {
                appleobj->attachCamera(camera);
            }
        }

        if (!Window::getPause()) {
            if (Input::pressed(SDLK_w)) {
                camera->setPosition(camera->getPosition() + glm::vec3(camera_front.x, camera_front.y, camera_front.z) * deltaTime * cameraspeed);
            }
            if (Input::pressed(SDLK_s)) {
                camera->setPosition(camera->getPosition() - glm::vec3(camera_front.x, camera_front.y, camera_front.z) * deltaTime * cameraspeed);
            }
            if (Input::pressed(SDLK_a)) {
                camera->setPosition(camera->getPosition() - glm::vec3(camera_right.x, camera_right.y, camera_right.z) * deltaTime * cameraspeed);
            }
            if (Input::pressed(SDLK_d)) {
                camera->setPosition(camera->getPosition() + glm::vec3(camera_right.x, camera_right.y, camera_right.z) * deltaTime * cameraspeed);
            }

            if (Input::pressed(SDLK_u)) {
                appleobj->setPosition(appleobj->getPosition() + glm::vec3(0, 0, -1) * deltaTime * cameraspeed);
            }
            if (Input::pressed(SDLK_h)) {
                appleobj->setPosition(appleobj->getPosition() + glm::vec3(-1, 0, 0) * deltaTime * cameraspeed);
            }
            if (Input::pressed(SDLK_k)) {
                appleobj->setPosition(appleobj->getPosition() + glm::vec3(1, 0, 0) * deltaTime * cameraspeed);
            }
            if (Input::pressed(SDLK_j)) {
                appleobj->setPosition(appleobj->getPosition() + glm::vec3(0, 0, 1) * deltaTime * cameraspeed);
            }

            if (Input::pressed(SDLK_c)) {
                camera->setZoom(0.2f);
            } else {
                camera->setZoom(1.0f);
            }

            if (Input::jclicked(SDL_BUTTON_MIDDLE)) {
                appleobj->getPhysicsObject()->applyForce(glm::vec3(0, jumpforce * 5, -jumpforce * 5));
            }
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

        gm.UpdatePhysics(deltaTime);
        gm.Update(light);
        // ScriptSystem::instance().updateScript("AppleTest", appleobj, deltaTime);

        // ResourceManager::getShader("octoline")->use();
        // ResourceManager::getShader("octoline")->uniformMatrix("projviewmodel", Renderer::getCamera()->getProjection() * Renderer::getCamera()->getView() * glm::mat4(1.0f));
        // rp3d::DebugRenderer& dbgrndr = gm.getPhysicsEngine()->getPhysicsWorld()->getDebugRenderer();

        // ResourceManager::getShader("octoline")->uniformVec3("clr", glm::vec3(0.2f, 1.0f, 0.2f));
        // int attrs[2] = { 3,  0 };

        // if (dbgrndr.getNbLines() != 0) {
        //     std::vector<float> lines;
        //     lines.reserve(dbgrndr.getNbLines() * 2 * 3);
        //     int i = 0;
        //     for (rp3d::DebugRenderer::DebugLine line : dbgrndr.getLines()) {
        //         lines.push_back(line.point1.x);
        //         lines.push_back(line.point1.y);
        //         lines.push_back(line.point1.z);

        //         lines.push_back(line.point2.x);
        //         lines.push_back(line.point2.y);
        //         lines.push_back(line.point2.z);
        //         i += 6;
        //     }

        //     Mesh* _mesh = new Mesh(lines, dbgrndr.getNbLines(), attrs);
        //     _mesh->draw(GL_LINES);
        //     delete _mesh;
        // }

        // ResourceManager::getShader("octoline")->uniformVec3("clr", glm::vec3(1.0f, 0.2f, 0.0f));
        // if (dbgrndr.getNbTriangles() != 0) {
        //     std::vector<float> triangles;
        //     triangles.reserve(dbgrndr.getNbTriangles() * 3 * 3);
        //     int i = 0;
        //     for (rp3d::DebugRenderer::DebugTriangle trian : dbgrndr.getTriangles()) {
        //         triangles.push_back(trian.point1.x);
        //         triangles.push_back(trian.point1.y);
        //         triangles.push_back(trian.point1.z);

        //         triangles.push_back(trian.point2.x);
        //         triangles.push_back(trian.point2.y);
        //         triangles.push_back(trian.point2.z);

        //         triangles.push_back(trian.point3.x);
        //         triangles.push_back(trian.point3.y);
        //         triangles.push_back(trian.point3.z);

        //         i += 9;
        //     }
            
        //     Mesh* _mesh = new Mesh(triangles, dbgrndr.getNbTriangles() * 3, attrs);
        //     _mesh->draw(GL_LINES);
        //     delete _mesh;
        // }

        Window::startFrame();        // ImGui новый кадр
        
        // рисуем маленькое overlay окно в углу
        {
            // ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x- 10, 10), ImGuiCond_Always);
            ImVec2 window_pos = ImVec2(ImGui::GetIO().DisplaySize.x - 5, 5);
            ImVec2 window_pos_pivot = ImVec2(1.0f, 0.0f); // 1.0 = справа, 0.0 = сверху
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
            ImGui::SetNextWindowBgAlpha(0.3f); // прозрачность
            if (ImGui::Begin("FPS Overlay", nullptr,
                            ImGuiWindowFlags_NoDecoration |
                            ImGuiWindowFlags_NoBackground |
                            ImGuiWindowFlags_AlwaysAutoResize |
                            ImGuiWindowFlags_NoSavedSettings |
                            ImGuiWindowFlags_NoFocusOnAppearing |
                            ImGuiWindowFlags_NoNav))
            {
                ImGuiIO& io = ImGui::GetIO();
                // ImGui::Text("FPS: %.1f", io.Framerate);
                float fps = io.Framerate;
                if (fps < 30) {
                    ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "FPS: %.1f", fps);
                } else if (fps < 60) {
                    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.4f, 1.0f), "FPS: %.1f", fps);
                } else {
                    ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "FPS: %.1f", fps);
                }
            }
            ImGui::End();
        }

        Window::renderGUI();         // рендер GUI

        Window::swapBuffers();
        Input::pullEvents();
    }

    ResourceManager::cleanup();
    Input::cleanup();
    Window::exit();

    return 0;
}