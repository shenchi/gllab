list(APPEND CMAKE_CXX_FLAGS "-std=c++0x")

file (GLOB SOURCE_FILES core/*.cpp *.cpp)

add_library (core ${SOURCE_FILES})