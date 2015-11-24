gllab
=====

This is an experimental project for me to learn how to write a 3D graphics engine. I'm trying to implement all the necessary basic functions and various advanced features for it (there is a list below). 

plan ( a wish list :) )
-----
* scene manager
	* scene graph
	* culling with BVH, BSP, Octree
	* draw call mangement
* lighting & effect
	* basic local illumination
	* Deffered Shading
	* Shadow Map and Reflective Shadow Map (RSM)
	* Splatting Indirect Illumination (SII, with RSM)
	* AO (SSAO, HBAO or others)
	* HDR tone mapping
	* Image-Based Lighting (IBL)
* optimization
	* memory optimization
	* transform calculations optimization
	* rendering queue
	* draw call batching
* multi-API support
	* DirectX 11
	* DirectX 12 or Metal

build & run
-----
First, please make sure you get `glfw3`, `glm`, `assimp`, `freeimage` installed.

go to the root folder of the project:
```Bash
# make a folder for building
mkdir build
cd build
cmake ..
make
./samples/[Sample Name]
```
