/**
 * @file renderer.h
 * @author Vlad Kartsaev
 * @brief Voxel generation functions
 * @version 0.8
 * @date 2023-04-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "../gamesystems/camera.h"
#include "meshmodel.h"

class MeshModel;
class Camera;

class Renderer {
private:
	static Camera* camera;

	static void vertex(std::vector<float>& posBuffer, std::vector<int8_t>& normalBuffer, std::vector<uint8_t>& colorBuffer, float x, float y, float z, const glm::ivec3 &normal, Color& clr);

	static void top(std::vector<float>& posBuffer, std::vector<int8_t>& normalBuffer, std::vector<uint8_t>& colorBuffer, float x, float y, float z, Color& clr);
	static void bottom(std::vector<float>& posBuffer, std::vector<int8_t>& normalBuffer, std::vector<uint8_t>& colorBuffer, float x, float y, float z, Color& clr);
	static void left(std::vector<float>& posBuffer, std::vector<int8_t>& normalBuffer, std::vector<uint8_t>& colorBuffer, float x, float y, float z, Color& clr);
	static void right(std::vector<float>& posBuffer, std::vector<int8_t>& normalBuffer, std::vector<uint8_t>& colorBuffer, float x, float y, float z, Color& clr);
	static void front(std::vector<float>& posBuffer, std::vector<int8_t>& normalBuffer, std::vector<uint8_t>& colorBuffer, float x, float y, float z, Color& clr);
	static void back(std::vector<float>& posBuffer, std::vector<int8_t>& normalBuffer, std::vector<uint8_t>& colorBuffer, float x, float y, float z, Color& clr);

public:
	static void init();

    static void addCamera(Camera* cam);

	static MeshModel* generateMeshModel(VoxelModel* voxelmodel);
    static Mesh* generateMesh(VoxelModel* voxels, VoxelModel::ChunkType& chunk);

    static Camera* getCamera();
};