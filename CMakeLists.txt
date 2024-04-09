cmake_minimum_required(VERSION 3.5.0)
project(
	VoxelX
	VERSION 0.2.5
	DESCRIPTION "VoxelX engine"
	LANGUAGES CXX
)

add_subdirectory(lib/)
add_subdirectory(editor/)
add_subdirectory(test/)