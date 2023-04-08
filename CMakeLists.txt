cmake_minimum_required(VERSION 3.0.0)
project(voxel3d VERSION 0.0.4)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set(SOURCE 
    main.cpp 
	window.cpp 
	input.cpp 
	graphics/shader.cpp
	graphics/texture.cpp
	graphics/mesh.cpp
	graphics/voxelrenderer.cpp
	graphics/vox_renderer.cpp
	voxels/voxel.cpp
	voxels/chunk.cpp
	voxels/chunks.cpp
	loaders/png_loading.cpp
	camera.cpp
	gameobject.cpp
)

set(HEADER
    window.h
	input.h 
	graphics/shader.h 
	graphics/texture.h 
	graphics/mesh.h
	graphics/voxelrenderer.h
	graphics/vox_renderer.h
	voxels/voxel.h
	voxels/chunk.h
	voxels/chunks.h
	loaders/png_loading.h
	camera.h
	gameobject.h
)

find_package(PNG REQUIRED)
if(NOT PNG_FOUND)
	message(SEND_ERROR "Failed to find PNG")
	return()
else()
	include_directories(${PNG_INCLUDE_DIR})
endif()

find_package(GLEW REQUIRED)
if(NOT GLEW_FOUND)
	message(SEND_ERROR "Failed to find GLEW")
	return()
else()
	include_directories(${GLEW_INCLUDE_DIR})
endif()

find_package(glfw3 3.3 REQUIRED)
if(NOT glfw3_FOUND)
	message(SEND_ERROR "Failed to find GLFW")
	return()
else()
	include_directories(${glfw3_INCLUDE_DIR})
endif()

find_package(OpenGL REQUIRED)
if(NOT OpenGL_FOUND)
	message(SEND_ERROR "Failed to find OpenGL")
	return()
else()
	include_directories(${OpenGL_INCLUDE_DIR})
endif()
find_package(GLU REQUIRED)

add_executable(voxel3d ${SOURCE})

target_link_libraries(voxel3d 
glfw
${PNG_LIBRARY}
${GLEW_LIBRARIES}
${OPENGL_gl_LIBRARY}
${GLU_LIBRARY}
)