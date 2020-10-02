cmake_minimum_required(VERSION 3.13)
project(voxels)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/libs/modules/")
set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(LibNoise REQUIRED)
add_subdirectory("libs/noiseutils/")

file(GLOB_RECURSE sources "src/*.cpp")

add_executable(voxels ${sources})

add_dependencies(voxels noiseutils)
target_compile_options(voxels PUBLIC -Wall)
target_include_directories(voxels PUBLIC
	"src/"
	"libs/"
	${LIBNOISE_INCLUDE_DIR}
	"libs/noiseutils/"
)
target_link_libraries(voxels
	glfw GLEW GL
	${LIBNOISE_LIBRARIES}
	noiseutils
)
