cmake_minimum_required(VERSION 3.0.0)
project(voxel3d VERSION 0.1.5)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(SOURCE 
    src/main.cpp 

	src/window/newwindow.cpp 
	src/window/newinput.cpp 

	src/graphics/shader.cpp
	src/graphics/texture.cpp
	src/graphics/mesh.cpp
	src/graphics/renderer.cpp
	src/graphics/particles.cpp
	src/graphics/gui.cpp

	src/graphics/uvregion.cpp
	src/graphics/sprite.cpp
	src/graphics/batch3d.cpp
	src/graphics/batch2d.cpp

	src/voxels/voxel.cpp

	src/loaders/resourcemanager.cpp
	src/loaders/png_loading.cpp
	
	src/gamesystems/camera.cpp
	src/gamesystems/gameobject.cpp
)

set(HEADER
    src/window/newwindow.h
	src/window/newinput.h 

	src/graphics/shader.h 
	src/graphics/texture.h 
	src/graphics/mesh.h
	src/graphics/renderer.h
	src/graphics/particles.h
	src/graphics/gui.h

	src/graphics/uvregion.h
	src/graphics/sprite.h
	src/graphics/batch3d.h
	src/graphics/batch2d.h

	src/voxels/voxel.h

	src/loaders/resourcemanager.h
	src/loaders/png_loading.h

	src/gamesystems/camera.h
	src/gamesystems/gameobject.h
)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(OpenGL_GL_PREFERENCE GLVND)

find_package(GLEW REQUIRED)
if(NOT GLEW_FOUND)
	message(SEND_ERROR "Failed to find GLEW")
	return()
else()
	include_directories(${GLEW_INCLUDE_DIR})
endif()

find_package(SDL2 REQUIRED)
if(NOT SDL2_FOUND)
	message(SEND_ERROR "Failed to find SDL2")
	return()
else()
	include_directories(${SDL2_INCLUDE_DIR})
endif()

find_package(SDL2_ttf REQUIRED)
if(NOT SDL2_ttf_FOUND)
	message(SEND_ERROR "Failed to find SDL2_ttf")
	return()
else()
	include_directories(${SDL2_TTF_INCLUDE_DIRS})
endif()

find_package(OpenGL REQUIRED)
if(NOT OpenGL_FOUND)
	message(SEND_ERROR "Failed to find OpenGL")
	return()
else()
	include_directories(${OpenGL_INCLUDE_DIR})
endif()

find_package(GLU REQUIRED)

find_package(PNG REQUIRED)
if(NOT PNG_FOUND)
	message(SEND_ERROR "Failed to find Libpng")
	return()
else()
	include_directories(${PNG_INCLUDE_DIR})
endif()

add_executable(voxel3d ${SOURCE})
target_link_libraries(voxel3d 
${SDL2_LIBRARIES}
${SDL2_TTF_LIBRARIES}
${GLEW_LIBRARIES}
${OPENGL_gl_LIBRARY}
${GLU_LIBRARY}
${PNG_LIBRARY}
)
