#pragma once

#include "assetmanager/assetmanager.h"
#include "utilities/logger.hpp"
#include <iostream>
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
            structure->forEachChunk([&] (VoxelChunk& chunk, const ChunkCoord &chunkPos) {
                if (!chunk._remesh) { return; }
                remeshed++;
                
                // chunk.met = new VoxelMesh(structure, chunk);
                // VoxelMesh* newmesh = Renderer::generateMesh(structure, chunk);
                gamemodel->getModel()->set(&chunk, std::make_unique<VoxelMesh>(structure, chunk));
                chunk._remesh = false;
            });
            if (remeshed > 0) {
                Logger::instance().log(LogLevel::VERBOSE, "GameManager", "Remeshed: bryellow<", remeshed ,">");
            }

            gamemodel->getModel()->draw(GL_TRIANGLES, t.modelMatrix, AssetManager::instance().get<Shader>("voxel"));
        }
    }
private:

};