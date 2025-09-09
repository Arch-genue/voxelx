#pragma once

#include "imgui.h"

#include "gamesystems/gamemanager.h"
#include "graphics/mesh.hpp"

void ShowDebug() {
    Camera* camera = GameManager::instance().getCamera();
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
        ImGui::Text("VoxelX");
        ImGui::TextColored(ImVec4(0.4f, 0.4f, 1.0f, 1.0f), "Camera position: X: %.1f Y: %.1f Z: %.1f", camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);
        ImGui::TextColored(ImVec4(0.4f, 0.4f, 1.0f, 1.0f), "Camera target: X: %.1f Y: %.1f Z: %.1f", camera->getTarget().x, camera->getTarget().y, camera->getTarget().z);

        ImGuiIO& io = ImGui::GetIO();
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

void ShowInspector(std::vector<GameObject*>& objects, int& selectedIndex) {
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 400, ImGui::GetIO().DisplaySize.y - 500), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(400, 500), ImGuiCond_Always);

    ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    // Список объектов
    ImGui::Text("GameObjects:");
    for (int i = 0; i < objects.size(); i++) {
        if (ImGui::Selectable(objects[i]->getName().c_str(), selectedIndex == i)) {
            selectedIndex = i;
        }
    }

    ImGui::Separator();

    // Редактирование выбранного объекта
    if (selectedIndex >= 0 && selectedIndex < (int)objects.size()) {
        GameObject* obj = objects[selectedIndex];
        glm::vec3 tempPos = obj->getPosition();
        glm::vec3 tempPhysPos = obj->getPhysicsObject()->getPosition();
        glm::vec3 tempPhysForce = obj->getPhysicsObject()->getForce();
        glm::vec3 tempPhysVelocity = obj->getPhysicsObject()->getLinearVelocity();
        bool updlocalmass = false;
        float mass = obj->getPhysicsObject()->getRigidBody()->getMass();
        bool stop = obj->getPhysicsObject()->isStopped();
        
        if (ImGui::DragFloat3("Position", &tempPos[0], 0.1f)) {
            obj->setPosition(tempPos);
        }
        ImGui::Text("Physics");
        if (ImGui::DragFloat3("Phys Position", &tempPhysPos[0], 0.1f)) {
            obj->getPhysicsObject()->setPosition(tempPhysPos);
        }
        if (ImGui::DragFloat3("Force", &tempPhysForce[0], 0.1f)) {
            obj->getPhysicsObject()->applyForce(tempPhysForce);
        }
        if (ImGui::DragFloat3("Velocity", &tempPhysVelocity[0], 0.1f)) {
            obj->getPhysicsObject()->setLinearVelocity(tempPhysVelocity);
        }
        if (ImGui::DragFloat("Mass", &mass)){
            obj->getPhysicsObject()->getRigidBody()->setMass(mass);
        }

        if (ImGui::Checkbox("Stop", &stop)) {
            if (stop) {
                obj->getPhysicsObject()->stop();
            } else {
                obj->getPhysicsObject()->resume();
            }
        }
        if (ImGui::Checkbox("Update Local Mass", &updlocalmass)) {
            if (updlocalmass) {
                obj->getPhysicsObject()->getRigidBody()->updateMassFromColliders();
            }
        }
        
    }

    ImGui::End();
}

void DebugPhysics(float aspect) {
    auto& gm = GameManager::instance();
    Camera* camera = gm.getCamera();

    AssetManager::instance().get<Shader>("octoline")->use();
    AssetManager::instance().get<Shader>("octoline")->uniformMatrix("projviewmodel", camera->getProjection(aspect) * camera->getView() * glm::mat4(1.0f));
    rp3d::DebugRenderer& dbgrndr = gm.getPhysicsEngine()->getPhysicsWorld()->getDebugRenderer();

    AssetManager::instance().get<Shader>("octoline")->uniformVec3("clr", glm::vec3(0.2f, 1.0f, 0.2f));
    int attrs[2] = { 3,  0 };

    if (dbgrndr.getNbLines() != 0) {
        std::vector<float> lines;
        lines.reserve(dbgrndr.getNbLines() * 2 * 3);
        int i = 0;
        for (rp3d::DebugRenderer::DebugLine line : dbgrndr.getLines()) {
            lines.push_back(line.point1.x);
            lines.push_back(line.point1.y);
            lines.push_back(line.point1.z);

            lines.push_back(line.point2.x);
            lines.push_back(line.point2.y);
            lines.push_back(line.point2.z);
            i += 6;
        }

        Mesh* _mesh = new Mesh(lines, dbgrndr.getNbLines(), attrs);
        _mesh->draw(GL_LINES);
        delete _mesh;
    }

    AssetManager::instance().get<Shader>("octoline")->uniformVec3("clr", glm::vec3(1.0f, 0.2f, 0.0f));
    if (dbgrndr.getNbTriangles() != 0) {
        std::vector<float> triangles;
        triangles.reserve(dbgrndr.getNbTriangles() * 3 * 3);
        int i = 0;
        for (rp3d::DebugRenderer::DebugTriangle trian : dbgrndr.getTriangles()) {
            triangles.push_back(trian.point1.x);
            triangles.push_back(trian.point1.y);
            triangles.push_back(trian.point1.z);

            triangles.push_back(trian.point2.x);
            triangles.push_back(trian.point2.y);
            triangles.push_back(trian.point2.z);

            triangles.push_back(trian.point3.x);
            triangles.push_back(trian.point3.y);
            triangles.push_back(trian.point3.z);

            i += 9;
        }
        
        Mesh* _mesh = new Mesh(triangles, dbgrndr.getNbTriangles() * 3, attrs);
        _mesh->draw(GL_LINES);
        delete _mesh;
    }
}