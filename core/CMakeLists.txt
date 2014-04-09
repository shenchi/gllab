list(APPEND CMAKE_CXX_FLAGS "-std=c++0x")

add_library (core Camera.cpp Engine.cpp FrameBuffer.cpp Material.cpp Mesh.cpp Object.cpp Program.cpp SceneObject.cpp Shader.cpp Texture.cpp UniformBuffer.cpp VertexBuffer.cpp)