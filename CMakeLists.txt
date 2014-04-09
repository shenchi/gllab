cmake_minimum_required (VERSION 2.6)

list(APPEND CMAKE_CXX_FLAGS "-std=c++0x")

project (gllab)

include_directories ("${PROJECT_SOURCE_DIR}/3rd_party/glm" "${PROJECT_SOURCE_DIR}/3rd_party/assimp/include")

add_subdirectory(core)
add_subdirectory(samples)