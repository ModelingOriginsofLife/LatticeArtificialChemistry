LatticeArtificialChemistry
==========================

Chemistry on a 2D grid, with Moore-neighborhood bond lengths.

Compilation instructions:

On Linux:

* Install libwxgtk-2.8-dev and cmake (and probably some other things)
* cd build
* ccmake ../src
* c (configure)
* g (generate)
* make

Then just use make to rebuild after changes. (Read up on CMake if needed.)

On Windows:

* Install wxWidgets 2.8 and CMake
* Run CMake
* Specify the build folder and the source folder (src)
* Configure button. Choose your target environment (e.g. Visual Studio)
* Generate button
* Open the solution in Visual Studio to build
