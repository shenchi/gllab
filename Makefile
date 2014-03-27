CXX=g++
CXX11=-std=c++0x
CXX_FLAGS=$(CXX11) -I3rd_party/glm
LD_FLAGS=-lglfw3 -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

gllab : *.cpp *.h
	$(CXX) $(CXX_FLAGS) *.cpp $(LD_FLAGS) -o gllab

clean:
	rm gllab