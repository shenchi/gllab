gllab
=====

a lab code base for CG learning

plan ( a wish list :) )
-----
* refactor, refactor and refactor
* lighting & effect
	* basic local illumination
	* Deffered Shading
	* Shadow Map --> Reflective Shadow Map (RSM)
	* Splatting Indirect Illumination (SII, with RSM)
	* AO (SSAO, HBAO or others)
	* HDR tone mapping
	* Image-Based Lighting (IBL)
* optimization
	* memory optimization
	* transform calculations optimization
	* rendering queue
	* draw call batching
* scene manager

build & run
-----

at root folder:
```Bash
mkdir build
cd build
cmake ..
make
./samples/[Sample Name]
```