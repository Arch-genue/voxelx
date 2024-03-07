cmake_minimum_required(VERSION 3.5.0)
project(
	voxelx-editor
	VERSION 0.1
	DESCRIPTION "VoxelX Editor"
	LANGUAGES CXX
)

# set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# message(STATUS ${QT_VERSION_MAJOR})

find_package(Qt6 REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS OpenGL)
find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)

# find_package(OpenGL REQUIRED)
# find_package(GLEW REQUIRED)
# find_package(GLU REQUIRED)
find_package(Freetype REQUIRED)

# include_directories(${FREETYPE_INCLUDE_DIRS})

add_executable(voxelx-editor)

target_sources(voxelx-editor PRIVATE
    src/main.cpp
	src/mainwindow.h
    src/mainwindow.cpp
	src/voxopenglwidget.h
	src/voxopenglwidget.cpp
	src/ui_mainwindow.h
)

target_link_libraries(voxelx-editor PRIVATE Qt6::Widgets)
target_link_libraries(voxelx-editor PRIVATE Qt6::OpenGL)
target_link_libraries(voxelx-editor PRIVATE Qt6::OpenGLWidgets)
target_link_libraries(voxelx-editor PUBLIC ${FREETYPE_LIBRARY})
target_link_libraries(voxelx-editor PRIVATE voxelx)

# target_link_libraries(voxelx-editor PRIVATE Qt6)

include_directories(${CMAKE_SOURCE_DIR}/lib/src)

install(TARGETS voxelx-editor RUNTIME DESTINATION bin)