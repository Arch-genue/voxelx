/**
 * @file voxelx.h
 * @author Vlad Kartsaev
 * @brief Main header file
 * @version 0.1
 * @date 2024-04-04
 * 
 * @copyright Copyright (c) 2024 Forever.
 * 
 */
#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "window/window.h"
#include "window/input.h"

// #include "assetmanager/assetmanager.h"
// #include "gamesystems/gamemanager.h"

// #include "voxels/voxelstructure.h"
// #include "gamesystems/camera.h"

/**
 * @brief Версия Voxel3D Engine
 */
#define _VERSION "0.3.0 BETA"

/**
 * @brief Размера буфера рендера
 *
 */
#define _RENDERSIZE 4096 * 4096

/**
 * @brief Заголовок окна
 *
 */
#define TITLE "VoxelX 0.2"