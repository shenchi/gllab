cmake_minimum_required (VERSION 2.6)

list(APPEND CMAKE_CXX_FLAGS "-std=c++0x")

project (gllab)

include_directories ("${PROJECT_SOURCE_DIR}/3rd_party/rapidxml" "/usr/local/include")
link_directories ("/usr/local/lib")

add_subdirectory(engine)
add_subdirectory(samples)