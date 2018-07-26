# MapTextSynthesizer

MapTextSynthesizer is a program to dynamically generate synthetic images containing text, which appear to be from historical maps. The produced images serve as training data for a Convolutional Neural Network that recognizes text in scanned images of historical maps. (Data not intended for training a text detection model)

Earlier work on this project (including most of the source code development) occurred in a separate repository. That work can be found on branch 'dev' at [niehusst/opencv_contrib](https://github.com/niehusst/opencv_contrib/tree/dev).

## Getting Started

To run a sample usage of the code, fork or clone this repository, run **add make instructions** 

**add a c++ program to make and run as well.**

#### Prerequisites/Dependencies

* **Pango**, a text formatting library. It comes preinstalled on many Linux machines. Otherwise, follow download from their [website](https://www.pango.org/).
* **Cairo**, a vector graphics library. It comes preinstalled on many Linux machines. Otherwise, follow download from their [website](https://cairographics.org/).
* **OpenCV**, a computer vision repository. It can be found on [github](https://github.com/opencv) .
* **glib-2.0**, a low-level Gnome library. glib is a dependency of pango and cairo; by downloading the other libraries, you will get glib-2.0. Their website can be found [here](https://lazka.github.io/pgi-docs/GLib-2.0/index.html).

### Install with CMake on Linux

open install.sh using a text editor and type in corresponding paths

e.g. Edited Where is the source code: [path-to]/MapTextSynthesizer

`` $ ./install.sh ``

### Examples

#### Python

see samples/text_synthesizer.py

#### C++

see samples/text_synthesizer.cpp

##### Sample compiling steps:

(if using virtual env,) `` export PKG_CONFIG_PATH=[install_prefix]/share/pkgconfig ``

(if using virtual env,) `` export LD_LIBRARY_PATH="[install_prefix]/lib" ``

``g++ text_syntheziser.cpp `pkg-config --cflags --libs mtsynth` ``


## Authors

* **Ziwen Chen** - [arthurhero](https://github.com/arthurhero)
* **Liam Niehus-Staab** - [niehusst](https://github.com/niehusst)

## Acknowledgments

* [Jerod Weinman](https://github.com/weinman) for his unwavering support as a mentor and guide through this project.
* [Anguelos](https://github.com/anguelos) for a starting base synthetic image generator in their fork of [opencv_contrib](https://github.com/anguelos/opencv_contrib/blob/gsoc_final_submission/modules/text/samples/text_synthesiser.py) 
* [Behdad Esfahbod](https://github.com/behdad), a developer of both Pango and cairo, for a number of functions he wrote in [cairotwisted.c](https://github.com/phuang/pango/blob/master/examples/cairotwisted.c) which we use to curve pango text baselines using cairo.

