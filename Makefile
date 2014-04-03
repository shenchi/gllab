CXX=g++
CXX11=-std=c++0x
CXX_FLAGS=$(CXX11) -I3rd_party/glm -I3rd_party/assimp/include
LD_FLAGS=-lglfw3 -lz -L3rd_party/assimp/lib/osx -lassimp -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

gllab.out : *.cpp *.hpp
	$(CXX) $(CXX_FLAGS) *.cpp $(LD_FLAGS) -o gllab.out

clean:
	rm gllab.out
