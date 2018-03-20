# NanoMagView

Simple Application to View Output Files from NanoMagMC

## Requirements

* Qt (tested using 5.10.1)
* OpenGL libs
* HDF5 libs

## Installation

* Clone the directory and VFRendering submodule using:

`git clone --recursive https://github.com/waterswims/NanoMagView.git`

* Generate the makefile using:

`qmake NanoMagView.pro`

* Build the VFRendering library using:

`make buildVF`

* Build the application using:

`make`

## Acknowledgements

This project used the VFRendering library which can be found at https://github.com/spirit-code/VFRendering
