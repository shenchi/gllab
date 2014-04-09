list(APPEND CMAKE_CXX_FLAGS "-std=c++0x")

include_directories (AFTER "${PROJECT_SOURCE_DIR}/core")

if (APPLE)
	set (THIRD_PARTY_LIB_SUFFIX "/osx")
endif (APPLE)

link_directories("${PROJECT_SOURCE_DIR}/3rd_party/assimp/lib${THIRD_PARTY_LIB_SUFFIX}")

if (APPLE)
	find_library (COCOA Cocoa)
	find_library (OPENGL OpenGL)
	find_library (IOKIT IOKit)
	find_library (COREVIDEO CoreVideo)

	set (APPLE_FRAMEWORKDS ${COCOA} ${OPENGL} ${IOKIT} ${COREVIDEO})
endif (APPLE)

set (EXTRA_LIBS glfw3 z assimp ${APPLE_FRAMEWORKDS})

#==================================================
add_executable (DeferredShading1 deferred_shading1.cpp)
target_link_libraries (DeferredShading1 ${EXTRA_LIBS} core)