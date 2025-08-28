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

# === Dear ImGui ===
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/vendor/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# === FetchContent sol2 ===
include(FetchContent)

FetchContent_Declare(
    sol2
    GIT_REPOSITORY https://github.com/ThePhD/sol2.git
    GIT_TAG main # или master, если хочешь
)
FetchContent_MakeAvailable(sol2)

file(GLOB_RECURSE HEADER ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
list(REMOVE_ITEM SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -w")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

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

include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)

find_package(Lua REQUIRED)
include_directories(${LUA_INCLUDE_DIR})

# find_package(Lua REQUIRED)
# include_directories(voxelx PUBLIC ${LUA_INCLUDE_DIR})

# NEW PHYSICS LIBRARY
find_package ( ReactPhysics3D REQUIRED )

set (LIBRARIES 
	${SDL2_LIBRARIES}
	${SDL2_TTF_LIBRARIES}
	${GLEW_LIBRARIES}
	${OPENGL_gl_LIBRARY}
	${GLU_LIBRARY}
	${PNG_LIBRARY}
	${FREETYPE_LIBRARY}
	ReactPhysics3D::ReactPhysics3D
	${LUA_LIBRARIES}
	sol2
)

# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/luacpp/Source)

add_executable(voxelx 
	${SOURCE}
	${IMGUI_SOURCES}
)
# add_library(voxelx STATIC ${SOURCE})
target_link_libraries(voxelx PRIVATE ${LIBRARIES})

# add_library(imgui STATIC ${IMGUI_SOURCES})
# target_include_directories(voxelx
# 	PUBLIC vendor/luacpp/Source
# 	PUBLIC vendor/lua
# )

target_compile_options(voxelx PRIVATE -g)

# install(TARGETS voxelx
#     LIBRARY DESTINATION lib
#     ARCHIVE DESTINATION lib)