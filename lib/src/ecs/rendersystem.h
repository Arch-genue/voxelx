#pragma once

#include "assetmanager/assetmanager.h"
#include "utilities/logger.hpp"

class RenderSystem {
public:
    void update(std::unordered_map<Entity, Transform>& transforms, std::unordered_map<Entity, Render>& renders) {
        for (auto& [entity, render] : renders) {
            Transform& t = transforms[entity];
            GameModel* gamemodel = AssetManager::instance().get<GameModel>(render.modelName);
            if (gamemodel == nullptr) {
                continue;
            }
            int remeshed = 0;
            VoxelStructure* structure = gamemodel->getStructure();
            structure->eachChunk([&] (VoxelChunk& chunk) {
                if (!chunk._dirty) { return; }
                remeshed++;

                gamemodel->getModel()->set(&chunk, Renderer::generate_mesh(chunk));
                // gamemodel->getModel()->set(&chunk, std::make_unique<VoxelMesh>(chunk));
                chunk._dirty = false;
            });
            if (remeshed > 0) {
                Logger::instance().log(LogLevel::VERBOSE, "GameManager", "Remeshed: bryellow<", remeshed ,">");
            }

            gamemodel->getModel()->draw(GL_TRIANGLES, t.modelMatrix, AssetManager::instance().get<Shader>("voxel"));
        }
    }
private:

};