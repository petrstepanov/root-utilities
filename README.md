Helper Class for CERN ROOT Canvas
=================================

By default, ROOT graphics on the canvas scales according to the canvas size. The bigger the canvas size is - the larger corresponding fonts, paddings are. This approach works good for canvases that contain one or two sub-pads. However, when ROOT canvas is divided into a higher number of sub-pads, say, 3x3, 5x5 or more, following issues take place:

* Absolute pixel font size on every child pad becomes too small and almost unreadable.
* Font sizes are inconsistent across various canvas elements (no typographic scale).
* Text in the legend and statistic boxed becomes very small.
* By default, ROOT canvas fas a fixed left margin value. Therefore, the Y axis title sometimes gets pushed outside the canvas area.

Above issues are especially notable on screens with smaller resolution. For instance, let us create a test ROOT canvas divided into 4 sub-pads. On the screenshot below, canvas size is set to 800x600 pixels. 

<figure>
  <img src="https://raw.githubusercontent.com/petrstepanov/root-canvas-helper/main/resources/canvas-default.png" alt="Standard ROOT canvas with multiple sub pads" />
</figure>

It is quite clear that sharing the plots with such a small text is rather unacceptable. Often when sharing screen at the online conferences, participants can hardly see the results.

Indeed, ROOT allows for setting a fixed pixel size font. However, this is usually not enough. A nice looking multi-pad plot also requires tweaking of the number of the tick marks, adjusting frame margins, tick mark length, changing title font sizes, axis title spacing and many more. ROOT is a very flexible framework and majority of these things can be easily tweaked. But at what cost? It comes down to duplicating chunks of code for every particular canvas. Moreover, 

This library is aimed on fixing issues outlined above. Canvas Helper converts ROOT plots from fractional-based geometry to fixed pixel sizes. As a result, the plot looks more alike to what Gnuplot output is.:

<figure>
  <img src="https://raw.githubusercontent.com/petrstepanov/root-canvas-helper/main/resources/canvas-processed.png" alt="ROOT canvas processed with CanvasHelper" />
</figure>

A single line of code is required to make the change happen. We simply pass the canvas instance to the library after all the primitives were created:
```
CanvasHelper::getInstance()->addCanvas(yourCanvas);
```

List of Fetures
---------------
* Adding a joint title for canvas that was divided into multiple pads. This was frequently asked on the ROOT forum.
* Library also allows adding a subtitle to the ROOT canvas. This may be useful for plots that need some extra information.

<p align="center">
  <img width="85%" src="https://raw.githubusercontent.com/petrstepanov/root-canvas-helper/main/resources/multi-pad-canvas-title.png" alt="Add title for ROOT canvas with multiple pads" />
</p>

* Impemented rounding of the parameter values and errors inside the statistics box. Parameter values round to the first significant digit of their errors. This improves visual clarity of the data.

<p align="center">
  <img width="85%" src="https://raw.githubusercontent.com/petrstepanov/root-canvas-helper/main/resources/cern-root-parameter-values-rounding.png" alt="Rounding of the parameter values for ROOT statistics box" />
</p>

* Stat boxes and legends can be force aligned to canvas edges. Single edge or a combination of two (e.g. top & left, bottom & right) are supported.

* Another cool feature is that registered canvases and sub-pads automatically re-adjust all the dimensions and their primitives upon the resize event.

<p align="center">
  <img width="85%" src="https://raw.githubusercontent.com/petrstepanov/root-canvas-helper/main/resources/canvas-resize.png" alt="Resizing a ROOT canvas" />
</p>

Manual Installation
-------------------

To make use of this helper class, first make sure your ROOT environmant is set up in the shell process `source <your-root-install-location>/bin/thisroot.*`). It is required for the `root-config` executable to be included in the system `PATH` environment variable. Next check out the repository:
```
mkdir -p ~/Downloads
cd ~/Downloads
git clone https://github.com/petrstepanov/root-canvas-helper
cd ./root-canvas-helper/src
```

Next, enter the Cling interpreter shell and compile a shared library from sources:
```
root
.L CanvasHelper.cpp+
.q
```

Now that the shared library is compiled we install the `.so` library, `.pcm` dictionary and `*.h` header files. Having ROOT directories already sources in the system environment, we simply integrate the library, its public header, and other files into the ROOT system folder. Commands below may require root persissions `sudo`:
```
cp CanvasHelper*.so CanvasHelper*.pcm CanvasHelper*.d `root-config --libdir`
cp CanvasHelper*.h `root-config --incdir`
cp demo.cpp $ROOTSYS/macros
```

Now that the library is installed and `demo.cpp` script is located in ROOT `macros` folder, user should be able to run the `demo.cpp` independent of the current working folder location:
```
root demo.cpp
```

Installation with CMake
-----------------------

This type of installation is more advances but will require users to have `cmake` program installed on computer. Usually CMake is included in Linux "Development Tools" group package. Similarly, ROOT environment should be sourced in the current shell. We clone the project and create the out-of-source build folder:
```
mkdir -p ~/Downloads
cd ~/Downloads
git clone https://github.com/petrstepanov/root-canvas-helper
mkdir -p ./root-canvas-helper-build && cd ./root-canvas-helper-build
```

CMake file `CMakeLists.txt` in the project root folder includes directions to do build and install everything we need. Next we generate the `Makefile`, and invoke the `install` target that depends on other required targets (generate dictionary, create shared libaray, build and link the executable, install generated files in corresponding locations):
```
cmake ../root-canvas-helper
make
make install
```

Installation is complete. Now users should be able to run `root demo.cpp` command to check the library features.

Use library in a ROOT Script
------------------------------
After the library was installed, it needs to be loaded into the interpreter session in your ROOT script:
```
gSystem->Load("CanvasHelper_cpp.so");
```

Contribute and Integrate with Development Environment
-----------------------------------------------------

Add to a Makefile-Based Project
-------------------------------