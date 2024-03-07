cmake_minimum_required(VERSION 3.5.0)
project(
	voxelx
	VERSION 0.2.3
	DESCRIPTION "Videogame voxel engine"
	LANGUAGES CXX
)

# include(CTest)
# enable_testing()

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(SOURCE 
    src/main.cpp 

	src/window/window.cpp 
	src/window/input.cpp 

	src/graphics/types/material.cpp
	src/graphics/types/octotree.cpp
	src/graphics/shader.cpp
	src/graphics/texture.cpp
	src/graphics/mesh.cpp
	src/graphics/textmesh.cpp
	src/graphics/cube.cpp
	src/graphics/renderer.cpp
	src/graphics/voxelparticles.cpp
	# src/graphics/gui.cpp

	src/graphics/uvregion.cpp
	src/graphics/sprite.cpp
	src/graphics/batch3d.cpp
	src/graphics/batch2d.cpp

	src/voxels/voxel.cpp
	src/voxels/voxelmodel.cpp
	# src/voxels/particle.cpp
	src/voxels/particlesmodel.cpp

	src/loaders/resourcemanager.cpp
	src/loaders/png_loading.cpp

	src/physics/physics.cpp
	src/physics/physicsengine.cpp
	src/physics/boxcollider.cpp
	
	src/gamesystems/camera.cpp
	src/gamesystems/transformobject.cpp
	src/gamesystems/gameobject.cpp
	src/gamesystems/linesobject.cpp
	src/gamesystems/gamemanager.cpp

	src/system/taskscheduler.cpp

	src/utilities/utils.cpp
	src/utilities/vtype.cpp
	src/utilities/logger.cpp
)

set(HEADER
    src/window/window.h
	src/window/input.h 

	src/graphics/types/material.h
	src/graphics/types/octotree.h

	src/graphics/shader.h 
	src/graphics/texture.h 
	src/graphics/mesh.h
	src/graphics/textmesh.h
	src/graphics/cube.h
	src/graphics/renderer.h
	src/graphics/voxelparticles.h
	# src/graphics/gui.h

	src/graphics/uvregion.h
	src/graphics/sprite.h
	src/graphics/batch3d.h
	src/graphics/batch2d.h

	src/voxels/voxel.h
	src/voxels/voxelmodel.h
	src/voxels/particlesmodel.h

	src/loaders/resourcemanager.h
	src/loaders/png_loading.h

	src/physics/physics.h
	src/physics/physicsengine.h
	src/physics/boxcollider.h

	src/gamesystems/camera.h
	src/gamesystems/transformobject.h
	src/gamesystems/gameobject.h
	src/gamesystems/linesobject.h
	src/gamesystems/gamemanager.h

	src/system/taskscheduler.h

	src/utilities/utils.h
	src/utilities/vtype.h
	src/utilities/logger.h
)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -w")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Videoadapter, not software render
set(OpenGL_GL_PREFERENCE GLVND)

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIR})

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_TTF_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIR})

find_package(GLU REQUIRED)

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})

set (LIBRARIES 
	${SDL2_LIBRARIES}
	${SDL2_TTF_LIBRARIES}
	${GLEW_LIBRARIES}
	${OPENGL_gl_LIBRARY}
	${GLU_LIBRARY}
	${PNG_LIBRARY}
	${FREETYPE_LIBRARY}

	# ${Boost_LIBRARIES}
	# ${Python3_LIBRARY}
)

# add_executable(voxelx ${SOURCE})
add_library(voxelx SHARED ${SOURCE})
target_link_libraries(voxelx PRIVATE ${LIBRARIES})
target_compile_options(voxelx PRIVATE -g)

install(TARGETS voxelx
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)