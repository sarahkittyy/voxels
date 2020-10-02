project(voxels)

file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(noiseutils ${sources})

target_include_directories(noiseutils PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	${LIBNOISE_INCLUDE_DIR}
)

target_link_libraries(noiseutils ${LIBNOISE_LIBRARIES})
target_compile_options(noiseutils PUBLIC -Wno-reorder)
