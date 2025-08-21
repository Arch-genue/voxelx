cmake_minimum_required(VERSION 3.5.0)
project(
	voxelx-test
	VERSION 0.1
	DESCRIPTION "VoxelX engine test program"
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(SOURCE 
    src/main.cpp 
)

set(HEADER
    src/main.h
)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -w")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

include_directories(${CMAKE_SOURCE_DIR}/lib/src)

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

set (LIBRARIES 
	# ${SDL2_LIBRARIES}
	# ${SDL2_TTF_LIBRARIES}
	${GLEW_LIBRARIES}
	${OPENGL_gl_LIBRARY}
	${GLU_LIBRARY}
	# ${FREETYPE_LIBRARY}
)

add_executable(voxelx-test ${SOURCE})
target_link_libraries(voxelx-test PRIVATE ${LIBRARIES})
target_link_libraries(voxelx-test PRIVATE voxelx)
target_compile_options(voxelx-test PRIVATE -g)

install(TARGETS voxelx-test
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)